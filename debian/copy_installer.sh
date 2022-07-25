if [[ x"$1" =~ x"v2" ]]
then
   echo "Copy python3-saithrift.install as python-saithrift.install"
   cp ./debian/installerFiles/python3-saithrift.install ./debian/python-saithrift.install
else
   if [ $(lsb_release -sr) -ge 11 ]
   then
      echo "Copy python3-saithrift.install as python-saithrift.install after Ver.11 releases"
      cp ./debian/installerFiles/python3-saithrift.install ./debian/python-saithrift.install
   else
      echo "Copy python2.7-saithrift.install as python-saithrift.install"
      cp ./debian/installerFiles/python2.7-saithrift.install ./debian/python-saithrift.install
   fi   
fi
