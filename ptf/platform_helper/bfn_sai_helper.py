import sys
sys.path.append("..")
from  sai_base_test import *

class BfnSaiHelper(SaiHelper):
    platform = 'bfn'
    
    def recreate_ports(self):
        print("BfnSaiHelper::recreate_ports")  