#!/bin/bash
DEVPATH="/usr/share/sonic/device"
REBOOT_CAUSE_FILE="/host/reboot-cause/reboot-cause.txt"
REBOOT_TIME=$(date)
REBOOT_METHOD="/sbin/kexec -e"
LOG_SSD_HEALTH="/usr/local/bin/log_ssd_health"
WATCHDOG_UTIL="/usr/local/bin/watchdogutil"
UIMAGE_HDR_SIZE=64

EXIT_SUCCESS=0
EXIT_FAILURE=1
EXIT_NOT_SUPPORTED=2
EXIT_FILE_SYSTEM_FULL=3
EXIT_NEXT_IMAGE_NOT_EXISTS=4

# Reboot immediately if we run the kdump capture kernel
VMCORE_FILE=/proc/vmcore
if [ -e $VMCORE_FILE -a -s $VMCORE_FILE ]; then
        echo "We have a /proc/vmcore, then we just kdump'ed"
        echo "User issued 'kdump' command [User: kdump, Time: ${REBOOT_TIME}]" > ${REBOOT_CAUSE_FILE}
        sync
        PLATFORM=$(grep -oP 'sonic_platform=\K\S+' /proc/cmdline)
        if [ ! -z "${PLATFORM}" -a -x ${DEVPATH}/${PLATFORM}/${PLAT_REBOOT} ]; then
            exec ${DEVPATH}/${PLATFORM}/${PLAT_REBOOT}
        fi
        # If no platform-specific reboot tool, just run /sbin/reboot
        /sbin/reboot
        echo 1 > /proc/sys/kernel/sysrq
        echo b > /proc/sysrq-trigger
fi

REBOOT_USER=$(logname)
PLATFORM=$(sonic-cfggen -H -v DEVICE_METADATA.localhost.platform)
ASIC_TYPE=$(sonic-cfggen -y /etc/sonic/sonic_version.yml -v asic_type)
VERBOSE=no
EXIT_NEXT_IMAGE_NOT_EXISTS=4
EXIT_SONIC_INSTALLER_VERIFY_REBOOT=21
SSD_FW_UPDATE="ssd-fw-upgrade"
REBOOT_SCRIPT_NAME=$(basename $0)
REBOOT_TYPE="${REBOOT_SCRIPT_NAME}"
PLATFORM_PLUGIN="${REBOOT_TYPE}_plugin"
BOOT_TYPE_ARG="soft"
TAG_LATEST=yes

function debug()
{
    if [[ x"${VERBOSE}" == x"yes" ]]; then
        echo `date` $@
    fi
    logger "$@"
}

function tag_images()
{
    if test -f /usr/local/bin/ctrmgr_tools.py
    then
        if [[ x"${TAG_LATEST}" == x"yes" ]]; then
            /usr/local/bin/ctrmgr_tools.py tag-all
        fi
    fi
}

function stop_pmon_service()
{
     CONTAINER_STOP_RC=0
     debug "Stopping pmon docker"
     docker kill pmon &> /dev/null || CONTAINER_STOP_RC=$?
     systemctl stop pmon || debug "Ignore stopping pmon error $?"
     if [[ CONTAINER_STOP_RC -ne 0 ]]; then
        debug "Failed killing container pmon RC $CONTAINER_STOP_RC ."
     fi
}

function stop_sonic_services()
{
    if [[ x"$ASIC_TYPE" != x"mellanox" ]]; then
        debug "Stopping syncd process..."
        docker exec -i syncd /usr/bin/syncd_request_shutdown --cold > /dev/null
        sleep 3
    fi
    stop_pmon_service
}

function clear_lingering_reboot_config()
{
    # Clear any outstanding warm-reboot config
    result=`timeout 10s config warm_restart disable; if [[ $? == 124 ]]; then echo timeout; else echo "code ($?)"; fi` || /bin/true
    debug "Cancel warm-reboot: ${result}"

    WARM_DIR="/host/warmboot"
    REDIS_FILE=dump.rdb
    TIMESTAMP=`date +%Y%m%d-%H%M%S`
    if [[ -f ${WARM_DIR}/${REDIS_FILE} ]]; then
        mv -f ${WARM_DIR}/${REDIS_FILE} ${WARM_DIR}/${REDIS_FILE}.${TIMESTAMP} || /bin/true
    fi
    /sbin/kexec -u || /bin/true
}

SCRIPT=$0

function show_help_and_exit()
{
    echo "Usage ${SCRIPT} [options]"
    echo "    Request rebooting the device. Invoke platform-specific tool when available."
    echo "    This script will shutdown syncd before rebooting."
    echo " "
    echo "    Available options:"
    echo "        -h, -? : getting this help"

    exit "${EXIT_SUCCESS}"
}

function setup_reboot_variables()
{
    # Kernel and initrd image
    NEXT_SONIC_IMAGE=$(sonic-installer list | grep "Next: " | cut -d ' ' -f 2)
    IMAGE_PATH="/host/image-${NEXT_SONIC_IMAGE#SONiC-OS-}"
    if grep -q aboot_platform= /host/machine.conf; then
        KERNEL_IMAGE="$(ls $IMAGE_PATH/boot/vmlinuz-*)"
        BOOT_OPTIONS="$(cat "$IMAGE_PATH/kernel-cmdline" | tr '\n' ' ') SONIC_BOOT_TYPE=${BOOT_TYPE_ARG}"
        INITRD=$(echo $KERNEL_IMAGE | sed 's/vmlinuz/initrd.img/g')
    elif grep -q onie_platform= /host/machine.conf; then
        if [ -r /host/grub/grub.cfg ]; then
            KERNEL_OPTIONS=$(cat /host/grub/grub.cfg | sed "/$NEXT_SONIC_IMAGE'/,/}/"'!'"g" | grep linux)
            KERNEL_IMAGE="/host$(echo $KERNEL_OPTIONS | cut -d ' ' -f 2)"
            BOOT_OPTIONS="$(echo $KERNEL_OPTIONS | sed -e 's/\s*linux\s*/BOOT_IMAGE=/') SONIC_BOOT_TYPE=${BOOT_TYPE_ARG}"
            INITRD=$(echo $KERNEL_IMAGE | sed 's/vmlinuz/initrd.img/g')
        # Handle architectures supporting Device Tree
        elif [ -f /sys/firmware/devicetree/base/chosen/bootargs ]; then
            KERNEL_IMAGE="$(ls $IMAGE_PATH/boot/vmlinuz-*)"
            BOOT_OPTIONS="$(cat /sys/firmware/devicetree/base/chosen/bootargs | sed 's/.$//') SONIC_BOOT_TYPE=${BOOT_TYPE_ARG}"
            INITRD=$(echo $KERNEL_IMAGE | sed 's/vmlinuz/initrd.img/g')

            # If initrd is a U-Boot uImage, remove the uImage header
            if file ${INITRD} | grep -q uImage; then
                INITRD_RAW=$(echo $KERNEL_IMAGE | sed 's/vmlinuz/initrd-raw.img/g')
                tail -c+$((${UIMAGE_HDR_SIZE}+1)) < ${INITRD} > ${INITRD_RAW}
                INITRD=${INITRD_RAW}
            fi
        else
            error "Unknown ONIE platform bootloader. ${REBOOT_TYPE} is not supported."
            exit "${EXIT_NOT_SUPPORTED}"
        fi
    else
        error "Unknown bootloader. ${REBOOT_TYPE} is not supported."
        exit "${EXIT_NOT_SUPPORTED}"
    fi
}

function load_kernel() {
    # Load kernel into the memory
    /sbin/kexec -l "$KERNEL_IMAGE" --initrd="$INITRD" --append="$BOOT_OPTIONS"
}

function reboot_pre_check()
{
    # Make sure that the file system is normal: read-write able
    filename="/host/test-`date +%Y%m%d-%H%M%S`"
    ERR=0
    touch ${filename} || ERR=$?
    if [[ ${ERR} -ne 0 ]]; then
        # Continue rebooting in this case, but log the error
        VERBOSE=yes debug "Filesystem might be read-only or full ..."
    fi
    rm ${filename}

    # Verify the next image by sonic-installer
    local message=$(sonic-installer verify-next-image 2>&1)
    if [ $? -ne 0 ]; then
        VERBOSE=yes debug "Failed to verify next image: ${message}"
        exit ${EXIT_SONIC_INSTALLER_VERIFY_REBOOT}
    fi
}

function parse_options()
{
    while getopts "h?v" opt; do
        case ${opt} in
            h|\? )
                show_help_and_exit
                ;;
            v )
                VERBOSE=yes
                ;;
            t )
                TAG_LATEST=no
                ;;
        esac
    done
}

parse_options $@

# Exit if not superuser
if [[ "$EUID" -ne 0 ]]; then
    echo "This command must be run as root" >&2
    exit "${EXIT_FAILURE}"
fi

#comment out for SAI testing, this command will response a error
#if [ -x ${LOG_SSD_HEALTH} ]; then
#    debug "Collecting logs to check ssd health before ${REBOOT_TYPE}..."
#    ${LOG_SSD_HEALTH}
#fi

debug "User requested rebooting device ..."

setup_reboot_variables
#comment out for SAI testing
#reboot_pre_check

# Tag remotely deployed images as local
#comment out for SAI testing
#tag_images

# Stop SONiC services gracefully.
#comment out for SAI testing, expect all the unnecessary will be stopped and removed in advance
#stop_sonic_services

#comment out for SAI testing
#clear_lingering_reboot_config

load_kernel

# Update the reboot cause file to reflect that user issued 'reboot' command
# Upon next boot, the contents of this file will be used to determine the
# cause of the previous reboot
echo "User issued '${REBOOT_SCRIPT_NAME}' command [User: ${REBOOT_USER}, Time: ${REBOOT_TIME}]" > ${REBOOT_CAUSE_FILE}

sync
sleep 3
sync

# sync the current system time to CMOS
#comment out for SAI testing
#if [ -x /sbin/hwclock ]; then
#    /sbin/hwclock -w || /bin/true
#fi
#
#if [ -x ${DEVPATH}/${PLATFORM}/${SSD_FW_UPDATE} ]; then
#    debug "updating ssd fw for${REBOOT_TYPE}"
#    ${DEVPATH}/${PLATFORM}/${SSD_FW_UPDATE} ${REBOOT_TYPE}
#fi

# Enable Watchdog Timer
#if [ -x ${WATCHDOG_UTIL} ]; then
#    debug "Enabling Watchdog before ${REBOOT_TYPE}"
#    ${WATCHDOG_UTIL} arm
#fi

# Run platform specific reboot plugin
#if [ -x ${DEVPATH}/${PLATFORM}/${PLATFORM_PLUGIN} ]; then
#    debug "Running ${PLATFORM} specific plugin..."
#    ${DEVPATH}/${PLATFORM}/${PLATFORM_PLUGIN}
#fi

# Reboot: explicitly call Linux "kexec -e"
debug "Rebooting with ${REBOOT_METHOD} to ${NEXT_SONIC_IMAGE} ..."
exec ${REBOOT_METHOD}

# Should never reach here
error "${REBOOT_TYPE} failed!"
exit "${EXIT_FAILURE}"
