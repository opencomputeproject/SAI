#!/bin/bash

#Services can be started, they must be in order
services=("swss" "syncd" "radv" "lldp" "dhcp_relay" "teamd" "bgp" "pmon" "telemetry" "acms" "snmp")
service_dir='/lib/systemd/system'
op_service(){
    if [[ x"$op" == x"remove" ]]; then
        if [[ ! -d "~/svcbak" ]]; then
            mkdir ~/svcbak
        fi
        echo "remove service from : /etc/systemd/system/sonic.target.wants"
        mv -f /etc/systemd/system/sonic.target.wants ~
    fi
    if [[ x"$op" == x"restore" ]]; then
        echo "restore service from : /etc/systemd/system/sonic.target.wants"
        cp -rf ~/sonic.target.wants /etc/systemd/system/sonic.target.wants
    fi
    for serv in ${services[*]}; do
        if [[ x"$skip" =~ x"$serv" ]]; then
            echo "Skip [$op] for service [$serv]."
        else
            if [[ x"$op" == x"start" || x"$op" == x"stop" ]]; then
                echo "[$op] service: [$serv]."
                systemctl $op $serv
            else
                if [[ x"$op" == x"remove" ]]; then
                    remove_service
                else
                    restore_service
                fi
            fi
        fi
    done

}

remove_service(){
    echo "remove service: [$serv]."
    mv $service_dir/$serv.service ~/svcbak
}

restore_service(){
    echo "restore service: [$serv]."
    cp ~/svcbak/$serv.service $service_dir/$serv.service 
}


check_ops() {
    # Print helpFunction in case parameters are empty
    if [ -z "$op" ]; then
        echo "Some or all of the parameters are empty";
        helpFunction
    fi

    if [[ x"$op" != x"start" && x"$op" != x"stop" && x"$op" != x"remove" && x"$op" != x"restore" ]]; then
        echo ""
        echo "Error: Operation perameters is not right, it only can be [stop|start|remove|restore].";
        helpFunction
    fi
}


helpFunction()
{
   echo ""
   echo "Use to operation on services list:"
   echo  ${services[*]}
   echo -e "\t-o [start|stop|restart|remove|restore] : start or stop or restart or remove or restore"
   echo -e "\t-s [service name] : the service names in the services list. It can be like [swss;syncd]"
   
   exit 1 # Exit script after printing help
}

while getopts ":o:s:" args; do
    case $args in
        o|operation)
            op=${OPTARG} 
            ;;
        s|skip)
            skip=${OPTARG} 
            ;;
        *)
            helpFunction
        ;;
    esac
done

check_ops
op_service
