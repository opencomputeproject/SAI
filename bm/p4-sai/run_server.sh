set -m
rm -rf log.txt
sudo -v
# sudo ./simple_switch -i 0@sw_port0 -i 1@sw_port1 -i 2@sw_port2 -i 3@sw_port3 -i 4@sw_port4 -i 5@sw_port5 -i 6@sw_port6 -i 7@sw_port7 -i 8@cpu_port --log-file log --log-flush sai.json &
sudo ./simple_switch -i 0@sw_port0 -i 1@sw_port1 -i 2@sw_port2 -i 3@sw_port3 -i 4@sw_port4 -i 5@sw_port5 -i 6@sw_port6 -i 7@sw_port7 -i 8@cpu_port --log-file log --log-flush --pcap sai.json &
sleep 2
./runtime_CLI --pre SimplePreLAG < p4src/DefaultConfig.txt
./sswitch_CLI < p4src/DefaultConfigMirror.txt
fg