
import os
#  Auxillary functions
def ensure_dir(directory):
    if not os.path.exists(directory):
        os.makedirs(directory)

def get_canonical_c_name(name):
    return name.replace('.','_')
