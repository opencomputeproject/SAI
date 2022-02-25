# backup behavioral-model original configs
mv -f behavioral-model/configure.ac configure_bak.ac
mv -f behavioral-model/targets/Makefile.am Makefile_bak.am
cp -f p4-sai/bm_config/configure.ac behavioral-model/configure.ac
cp -f p4-sai/bm_config/Makefile.am behavioral-model/targets/Makefile.am

rm -rf behavioral-model/targets/p4-sai
mv p4-sai behavioral-model/targets/.
# make behavioral model, p4-sai target.

cd behavioral-model/
./autogen.sh
./configure
make

# return original config files 
cd -
mv -f Makefile_bak.am behavioral-model/targets/Makefile.am 
mv -f configure_bak.ac behavioral-model/configure.ac 

# return compiled target
mv behavioral-model/targets/p4-sai .

# edit relative paths
# cp simple_switch simple_switch_bak
sed -i 's/P4\/SAI\/bm\/behavioral-model\/targets\/p4-sai/p4-sai/g' simple_switch
sed -i 's/\.\.\/\.\.\/src\/bm_runtime\/\.libs\/libbmruntime\.a/\.\.\/behavioral-model\/src\/bm_runtime\/\.libs\/libbmruntime\.a/g' simple_switch
sed -i 's/\.\.\/\.\.\/thrift_src\/\.libs/\.\.\/behavioral-model\/thrift_src\/\.libs/g' simple_switch
rm runtime_CLI
ln -s ../behavioral-model/tools/runtime_CLI.py runtime_CLI