* Build prerequisites

    1. SAI header files should be installed in /usr/include/sai/

    2. Vender specific SAI library (-lsai)

    3. Apache thrift 0.9.2

    4. ctypesgen

* Build the saiserver and SAI thrift client python library

    make

* Run experiments

  Switch side:

    1. Install thrift library on the switch

    2. Copy saiserver binary to the switch

    3. Run sai server

      ./saiserver

  Server side:

    1. Install ptf on the client

    2. Install sai thrift client library on the server

         tar xzf saithrift-0.9.tar.gz
         cd saithrift-0.9
         python setup.py install

    3. Copy tests directory to client

    sudo ptf --test-dir tests switch.L3IPv4HostTest --interface '0@eth0' --interface '1@eth1' --interface '2@eth2' -t "server='10.0.0.1'"
