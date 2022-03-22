#!/bin/bash

#
# Prepare the script to start the saiserver services.
# Chaning the existing syncd services for saiservice container start.
# This script must be execute with root user
#

#Prepare the syncd service related files for saiserver
function copy_syncd_files()
{
    if [ ! -d "$SAISERVER_SCRIPT_ROOT" ]; then
        echo "make folder $SAISERVER_SCRIPT_ROOT..." .
    fi
    echo "copy_syncd_files"
    cp $SYNCD_SCRIPT_ROOT/usr/local/bin/syncd_common.sh $SAISERVER_COMMON
    cp $SYNCD_SCRIPT_ROOT/usr/local/bin/syncd.sh $SAISERVER_LOCAL
    cp $SYNCD_SCRIPT_ROOT/usr/lib/systemd/system/syncd.service $SAISERVER_SERVICE
    cp $SYNCD_SCRIPT_ROOT/usr/bin/syncd.sh $SAISERVER
}

function change_scripts()
{
    echo "change_scripts"
    #replace all the syncd to saiserver
    sed -i "s/syncd/saiserver/g" $SAISERVER_COMMON
    sed -i "s/syncd/saiserver/g" $SAISERVER_LOCAL
    sed -i "s/syncd/saiserver/g" $SAISERVER_SERVICE
    sed -i "s/syncd/saiserver/g" $SAISERVER
}

function comment_out_funs()
{
    echo "comment out functions and variables"

    #remove pmon
    sed -i "s/  \/bin\/systemctl start pmon/  #\/bin\/systemctl start pmon/g" $SAISERVER_LOCAL
    sed -i "s/  \/bin\/systemctl stop pmon/  #\/bin\/systemctl start pmon/g" $SAISERVER_LOCAL

    # remove deps
    sed -i "s/After=swss.service//g" $SAISERVER_SERVICE

    # remove lock check
    sed -i "s/  lock_service_state_change/ #lock_service_state_change/g" $SAISERVER_COMMON
    sed -i "s/  unlock_service_state_change/ #unlock_service_state_change/g" $SAISERVER_COMMON
    
    #other variables
    sed -i "s/PEER=\"swss\"/#PEER=\"swss\"/g" $SAISERVER_LOCAL
}

function change_saiserver_version()
{
    echo "change saiserver version to $version"
    sed -i "s/docker-saiserver/docker-saiserver$version/g" $SAISERVER   
}

check_versions() {
    # Print helpFunction in case parameters are empty
    if [ -z "$version" ]; then
        echo "version set to default v1.";
        version=
    fi

    #when v1 set the version to null
    if [[ x"$version" != x"v2" && x"$version" != x"" ]]; then
        echo ""
        echo "Error: Version perameters is not right, it only can be [v2].";
        helpFunction        
    fi
}


helpFunction()
{
   echo ""
   echo "Prepare the script to start the saiserver services:"
   echo -e "\t-v [v1|v2]: saiserver version, support v1 and v2"
   
   exit 1 # Exit script after printing help
}

while getopts ":v:" args; do
    case $args in
        v|operation)
            version=${OPTARG} 
            ;;
        *)
            helpFunction
        ;;
    esac
done

SYNCD_SCRIPT_ROOT=""
SAISERVER_SCRIPT_ROOT=""
SAISERVER_COMMON=$SAISERVER_SCRIPT_ROOT/usr/local/bin/saiserver_common.sh
SAISERVER_LOCAL=$SAISERVER_SCRIPT_ROOT/usr/local/bin/saiserver.sh
SAISERVER_SERVICE=$SAISERVER_SCRIPT_ROOT/usr/lib/systemd/system/saiserver.service
SAISERVER=$SAISERVER_SCRIPT_ROOT/usr/bin/saiserver.sh

check_versions
copy_syncd_files
change_scripts
comment_out_funs
change_saiserver_version

echo "Start saiserver service"
echo -e "\tsudo systemctl start saiserver"

echo "Start sai server manually, run inside saiserver container with:"
echo -e "\t/usr/bin/start.sh"
echo -e "\t/usr/sbin/saiserver -p /etc/sai.d/sai.profile -f /usr/share/sonic/hwsku/port_config.ini"
