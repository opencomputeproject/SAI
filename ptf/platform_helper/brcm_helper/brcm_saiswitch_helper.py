THIS_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(THIS_DIR, '..'))
from common_helper import saiswitch_helper
from sai_base_test import * # pylint: disable=wildcard-import; lgtm[py/polluting-import]
from saiswitch import * # pylint: disable=wildcard-import; lgtm[py/polluting-import]

class BrcmSaiSwitchHelper(saiswitch_helper):
    def __init__(self):
        pass

    
    def create_route_entry_from_default_vrf(self):
        route_entry = sai_thrift_route_entry_t(
            switch_id=self.switch_id,
            vr_id = self.default_vrf,
            destination=sai_ipprefix('0.0.0.0/0'))
        status = sai_thrift_create_route_entry(
            self.client, route_entry, next_hop_id=nhop)
        self.assertEqual(status, SAI_STATUS_SUCCESS)