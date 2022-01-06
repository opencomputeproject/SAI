import sys
sys.path.append("..")
from  sai_base_test import *

class CommonSaiHelper(SaiHelper):
    platform = 'common'

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
        TODO identify if it is a bug from intel
        when set SAI_FDB_ENTRY_TYPE_STATIC, allow_mac_move will be checked, and its default value is false
        then, when a port get different mac (port with different session), it should be dropped.
        but intel can pass the test
        '''
        print("CommonSaiHelper::sai_thrift_create_fdb_entry_allow_mac_move") 
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
            allow_mac_move=allow_mac_move)
