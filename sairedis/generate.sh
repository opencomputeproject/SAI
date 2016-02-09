#!/ust/bin/bash

set -e

cat *.h |perl -ne 'print uc "$2\n" if /typedef.+(sai_([_a-z]+)_fn)/i'|sort|uniq|wc
