# backup behavioral-model original configs
mv -f behavioral-model/configure.ac configure_bak.ac
mv -f behavioral-model/targets/Makefile.am Makefile_bak.am
cp -f p4-sai/bm_config/configure.ac behavioral-model/configure.ac
cp -f p4-sai/bm_config/Makefile.am behavioral-model/targets/Makefile.am

cp -rf p4-sai behavioral-model/targets/.
# make behavioral model, p4-sai target.

cd behavioral-model/
./autogen.sh
./configure
make

# return original config files 
cd -
mv -f p4-sai/bm_config/Makefile_bak.am behavioral-model/targets/Makefile.am 
mv -f p4-sai/bm_config/configure_bak.ac behavioral-model/configure.ac 