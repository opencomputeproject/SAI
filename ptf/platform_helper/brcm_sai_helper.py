import sys
# from common_sai_helper import CommonSaiHelper
sys.path.append("..")
from  sai_base_test import *
import time


class BrcmSaiHelper(common_sai_helper.CommonSaiHelper):
    platform = 'brcm'

    def remove_switch(self):
        '''
        Method to remove the switch.
        '''
        print("BrcmSaiHelperBase::remove_switch does not support. Cannot recreate after remove.") 


    def recreate_ports(self):
        print("BrcmSaiHelperBase::recreate_ports does not support.") 
    

    def sai_thrift_create_fdb_entry_allow_mac_move(self,
                                client,
                                fdb_entry,
                                type=None,
                                packet_action=None,
                                user_trap_id=None,
                                bridge_port_id=None,
                                meta_data=None,
                                endpoint_ip=None,
                                counter_id=None,
                                allow_mac_move=None):
        '''
        when set SAI_FDB_ENTRY_TYPE_STATIC, allow_mac_move will be checked, and its default value is false
        then, when a port get different mac (port with different session), it should be dropped.
        but intel can pass the test
        '''
        print("BrcmSaiHelper::sai_thrift_create_fdb_entry_allow_mac_move") 
        sai_thrift_create_fdb_entry(
            client=client, 
            fdb_entry=fdb_entry, 
            type=type, 
            packet_action=packet_action,
            user_trap_id=user_trap_id,
            bridge_port_id=bridge_port_id,
            meta_data=meta_data,
            endpoint_ip=endpoint_ip,
            counter_id=counter_id,
            allow_mac_move=True)

    
    def turn_up_and_check_ports(self):
        '''
        Method to turn up the ports.
        
        Needs the following class attributes:

            self.port_list - list of all active port objects
        '''
        retries = 10
        for port_id in self.port_list:
            try:
                sai_thrift_set_port_attribute(
                    self.client, port_oid=port_id, admin_state=True)
            except BaseException as e:
                print("Cannot setup port admin state, error {}".format(e))
        
        for num_of_tries in range(retries):
            all_ports_are_up = True
            time.sleep(2)
            for port_id in self.port_list:
                port_attr = sai_thrift_get_port_attribute(
                    self.client, port_id, oper_status=True)
                if port_attr['oper_status'] != SAI_PORT_OPER_STATUS_UP:
                    all_ports_are_up = False
                    time.sleep(1)
                    print("port is down: {}".format(port_attr['oper_status']))
            if all_ports_are_up:
                break
        if not all_ports_are_up:
            print("Not all the ports are up after {} rounds of retries.".format(retries))

    
    def start_switch(self):
        """
        Start switch and wait seconds for a warm up.
        """

        self.switch_id = sai_thrift_create_switch(
        self.client, init_switch=True, src_mac_address=ROUTER_MAC)
        self.assertEqual(self.status(), SAI_STATUS_SUCCESS)
        switch_init_wait = 5
        print("Waiting for switch to get ready, {} seconds ...".format(switch_init_wait))
        time.sleep(switch_init_wait)


    def check_cpu_port_hdl(self):
        """
        Checks cpu port handler, expect the cpu_port_hdl equals to qos_queue port id

        Needs the following class attributes:

            self.cpu_port_hdl - cpu_port_hdl id 

        Sets the following class attributes:

            self.cpu_port - cpu_port id 

        """
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.cpu_port_hdl,
                                             qos_number_of_queues=True)
        num_queues = attr['qos_number_of_queues']
        q_list = sai_thrift_object_list_t(count=num_queues)
        attr = sai_thrift_get_port_attribute(self.client,
                                             self.cpu_port_hdl,
                                             qos_queue_list=q_list)

        for queue in range(0, num_queues):
            queue_id = attr['qos_queue_list'].idlist[queue]
            setattr(self, 'cpu_queue%s' % queue, queue_id)
            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id,
                port=True,
                index=True,
                parent_scheduler_node=True)
            self.assertEqual(queue, q_attr['index'])
            # in broadcom platform, the q_attr["port"] is not equals to cpu_port_hdl
            # cpu_port_hdl is ahead of the cpu_port list
            self.cpu_port = q_attr["port"]
