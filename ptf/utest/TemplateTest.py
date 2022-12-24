from BasicTest import BasicMockedTest
# Try to import from local build folder
# If cannot import then try to import from global folder
try:
    from meta.sai_adapter import *
except ImportError:
    from sai_thrift.sai_adapter import *
from MockClient import MockSuccessClient
import unittest

"""
Class use to test the template.
Run command under folder 'meta'
    perl -Irpc gensairpc.pl --adapter_logger
    copy sai_adaptor to ./test/saithriftv2/gen-py/sai
    cd ./test/saithriftv2
    sudo python3 setup.py install
Install PTF
    git clone https://github.com/p4lang/ptf.git
    python3.7 setup.py install --single-version-externally-managed --record ./ptf_install.txt
Install scapy
Then check the result

"""
ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
LOG_FILE_DIR = os.path.join(ROOT_DIR, 'logs')
LOG_FILE_PATH = os.path.join(LOG_FILE_DIR, 'output.log')

class TestAdapterLogger(BasicMockedTest):
    
    def setUp(self):
        BasicMockedTest.setUp(self)
        self.client = MockSuccessClient()
        
    
    def test_logger(self):
        self.set_logger_name()
        sai_thrift_remove_acl_table(self.client, 11111)
        self.check_file_contains(LOG_FILE_PATH, 'acl_table_oid')
        
        sai_thrift_create_switch(self.client, 
                                 init_switch=True, 
                                 hardware_access_bus="11:11:11:11:11:11")
        self.check_file_contains(LOG_FILE_PATH, 'hardware_access_bus')
        sai_thrift_get_acl_table_attribute(self.client, acl_table_oid=1, acl_stage=1)
        self.check_file_contains(LOG_FILE_PATH, 'SAI_ACL_TABLE_ATTR_ACL_STAGE')
        
    
    def check_file_contains(self, file, content):
        with open(file) as f:
            datafile = f.readlines()
            found = False
            for line in datafile:
                if content in line:
                    return True
        return False
