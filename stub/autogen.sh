#!/bin/bash

# get two strings of version numbers
# parse the two strings and compare
function compare_versions() {
   cat <<EOF > /tmp/$$.awk
/[0-9]+\.[0-9]+(\.[0-9]+)?[    ]+[0-9]+\.[0-9]+(\.[0-9]+)?/{
  vb = "$2";
  nb = split(vb,b,"[.]");
  va = "$1";
  na = split(va,a,"[.]");
  for (i = na + 1; i < 4; i++) {
      a[i] = 0;
  }
  for (i = nb + 1; i < 4; i++) {
      b[i] = 0;
  }
  for (i = 1; i <= 3; i++) {
    if (b[i] < a[i]) {
#      print "FAIL:" va ">" vb " at index:" i;
        exit(1);
    } else if (b[i] > a[i]) {
#       print "OK:" va "<" vb " at index:" i;
       exit(0);
    }
  }
#  print "OK:" va "==" vb;
  exit(0);
}
{
  exit(1);
}
EOF
  echo "$1 $2 " | awk -f /tmp/$$.awk
  status=$?
  rm /tmp/$$.awk
  return $status
}

cd ${0%*/*}

# clean old autogen files
./autoclean.sh

# make sure autoconf is up-to-date
ac_ver=`autoconf --version | head -n 1 | awk '{print $NF}'`
compare_versions 2.59 $ac_ver
if test $? = 1; then
    echo Min autoconf version is 2.59
    exit 1
fi

# make sure automake is up-to-date
am_ver=`automake --version | head -n 1 | awk '{print $NF}'`
compare_versions 1.9.2 $am_ver
if test $? = 1; then
    echo Min automake version is 1.9.2
    exit 1
fi

# make sure libtool is up-to-date
lt_ver=`libtoolize --version | head -n 1 | awk '{print $4}'`
compare_versions 2.4.2 $lt_ver
if test $? = 1; then
    echo Min libtool version is 2.4.2
    exit 1
fi

# handle our own autoconf:
echo Visiting `pwd`
aclocal -I config 2>&1 |  grep -v "Warning: underquoted definition of"
libtoolize --force --copy
autoheader
automake --add-missing --foreign --copy --force
autoconf
