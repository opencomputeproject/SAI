if [[ x"$1" =~ x"v2" ]]
then
   if [ $(lsb_release -sr) -ge 12 ]
   then
      echo "Copy python3-saithrift-bookworm.install as python-saithrift.install after Ver.12 releases"
      cp ./debian/installerFiles/python3-saithrift-bookworm.install ./debian/python-saithrift.install
   else
      echo "Copy python3-saithrift.install as python-saithrift.install"
      cp ./debian/installerFiles/python3-saithrift.install ./debian/python-saithrift.install
   fi
else
   if [ $(lsb_release -sr) -ge 12 ]
   then
      echo "Copy python3-saithrift-bookworm.install as python-saithrift.install after Ver.12 releases"
      cp ./debian/installerFiles/python3-saithrift-bookworm.install ./debian/python-saithrift.install
   elif [ $(lsb_release -sr) -eq 11 ]
   then
      echo "Copy python3-saithrift.install as python-saithrift.install for Ver.11 releases"
      cp ./debian/installerFiles/python3-saithrift.install ./debian/python-saithrift.install
   else
      echo "Copy python2.7-saithrift.install as python-saithrift.install"
      cp ./debian/installerFiles/python2.7-saithrift.install ./debian/python-saithrift.install
   fi   
fi
