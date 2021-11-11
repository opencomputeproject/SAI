import sys
sys.path.append("..")
from  sai_base_test import *

class BrcmSaiHelper(SaiHelper):
    platform = 'brcm'
    
    def recreate_ports(self):
        print("BrcmSaiHelperBase::recreate_ports does not support.") 


    def check_cpu_port_hdl(self, num_queues):
        for queue in range(0, num_queues):
            queue_id = attr['qos_queue_list'].idlist[queue]
            setattr(self, 'cpu_queue%s' % queue, queue_id)
            q_attr = sai_thrift_get_queue_attribute(
                self.client,
                queue_id,
                port=True,
                index=True,
                parent_scheduler_node=True)
            self.assertTrue(queue == q_attr['index'])
            #in broadcom platform, the q_attr["port"] is not equals to cpu_port_hdl
            # cpu_port_hdl is ahead of the cpu_port list
            self.cpu_port = q_attr["port"]
