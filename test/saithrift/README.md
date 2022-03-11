# Build prerequisites: 
1. python 2.7

2. SAI header files should be installed in /usr/include/sai/

   *You can use the header files in folder [inc](../../inc)*    
**Different SAI branch maps to different SAI header, make sure you use the right version of SAI header.**

3. Vendor specific SAI library (ld -lsai)

4. Apache thrift 0.11.0

   You can download thrift source packages from: http://archive.apache.org/dist/thrift/
  
   Installation instructions at: https://thrift.apache.org/docs/install/
  
   *Note. Use: ./configure --prefix=/usr . The default is to install in /usr/local*
   Please refer to ./configure --help for more details.
  
   **make sure install libboost with version 1.71 (1.71 got verified) when run './configure'**
   ```
   #For example, if don't want to build with java and php
   ./configure  --with-java=no  --with-php=no --prefix=/usr
   ```

5. ctypesgen: sudo -H pip install ctypesgen

   Note. It is also desired to install the doxygen package: sudo apt install doxygen

# Build the saiserver and SAI thrift client python library

    make

# Run experiments

Note. The assumption is that your build machine, test machine (test client) and switch (where SAI is executed) are based on the same Linux distribution.
Otherwise, please setup an appropriate cross-compile environment to generate 'saiserver' and the 'libthrift-0.11.0.so' to be executed on the switch (test server).

## Switch (server) side:

1. Install thrift library on the switch

   For instance, you can copy libthrift-0.11.0.so obtained at step 3 above to: /usr/lib/x86_64-linux-gnu or /usr/lib (on the switch)

2. Copy saiserver binary to the switch

3. Run sai server
    Please make sure that you have all needed libraries (.so) on the switch:
    ```
    ldd saiserver
    ```
    If all libraries are present, execute:
    ```
    ./saiserver -p profile.ini -f portmap.ini
    ```

    *You can find a sample configuration for mellanox sn2700 under src/msn_2700 directory*

## Client side (test machine):

1. Install ptf on the client
    Use the link to PTF - e.g. ptf @ fe3c89 provided at:

    https://github.com/opencomputeproject/SAI/tree/master/test

    or the desired branch.

    You must clone the ptf repo separately. For instance:

    git clone https://github.com/p4lang/ptf.git

    Note. Always look for the latest link provided at https://github.com/opencomputeproject/SAI/tree/master/test.

    Use the 'ptf' Python script in the cloned directory.

2. Install the SAI thrift client library on the (client) test machine

   Use the source code in: SAI/test/saithrift
   ```
   sudo python setup.py install
   ```

3. Copy tests directory to the test machine (client)
    ```
    sudo ptf --test-dir tests sail3.L3IPv4HostTest --interface '0@eth0' --interface '1@eth1' --interface '2@eth2' -t "server='10.0.0.1';port_map_file='default_interface_to_front_map.ini'"
    ```

    sample configuration for mellanox sn2700 under src/msn_2700 directory: default_interface_to_front_map.ini

    **Note. Existing test cases are stored in the directory:** `SAI/test/saithrift/tests`


    sail3.L3IPv4HostTest refers to file: ./tests/sail3.py, test case testL3IPv4HostTest
    Examine the sail3.py file for details.

    eth1 and eth2 in the command above refer to the client (test machine) interfaces.

    server='10.0.0.1' is the IP address of the switch (server) - it must be accessible from the test machine (client)

3. Copy tests directory to client

4. Copy generated src/gen-py directory to client

