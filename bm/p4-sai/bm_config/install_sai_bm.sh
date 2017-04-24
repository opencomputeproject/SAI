# backup behavioral-model original configs
mv -f ../../behavioral-model/configure.ac configure_bak.ac
mv -f ../../behavioral-model/targets/Makefile.am Makefile_bak.am
cp -f configure.ac ../../behavioral-model/configure.ac
cp -f Makefile.am ../../behavioral-model/targets/Makefile.am

# make behavioral model, p4-sai target.
#cd ../../behavioral-model/
#./autogen.sh
#./configure
#make

# return original config files 
#cd -
#mv -f Makefile_bak.am ../../behavioral-model/targets/Makefile.am 
#mv -f configure_bak.ac ../../behavioral-model/configure.ac 