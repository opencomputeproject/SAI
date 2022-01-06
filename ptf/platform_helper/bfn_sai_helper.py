import sys
# from common_sai_helper import CommonSaiHelper
sys.path.append("..")
from  sai_base_test import *


class BfnSaiHelper(common_sai_helper.CommonSaiHelper):
    platform = 'bfn'
    
    def recreate_ports(self):
        print("BfnSaiHelper::recreate_ports")            
        if 'port_config_ini' in self.test_params:
            if 'createPorts_has_been_called' not in config:
                self.createPorts()
                # check if ports became UP
                #self.checkPortsUp()
                config['createPorts_has_been_called'] = 1
        wait_sec = 5
        print("Waiting for ports to get ready, {} seconds ...".format(wait_sec))
        time.sleep(wait_sec)
