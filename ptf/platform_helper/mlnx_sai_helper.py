import sys
# from common_sai_helper import CommonSaiHelper
sys.path.append("..")
from  sai_base_test import *


class MlnxSaiHelper(common_sai_helper.CommonSaiHelper):
    platform = 'mlnx'