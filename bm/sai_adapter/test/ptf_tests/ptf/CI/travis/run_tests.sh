#!/usr/bin/env bash

sudo python ptf_nn/ptf_nn_agent.py \
    --device-socket 0@tcp://127.0.0.1:10001 -i 0-1@veth0 \
    &>/dev/null &

sleep 5

sudo python ptf_nn/ptf_nn_agent.py \
    --device-socket 1@tcp://127.0.0.1:10002 -i 1-1@veth3 \
    &>/dev/null &

sleep 5

sudo python ptf_nn/ptf_nn_test_bridge.py -ifrom veth1 -ito veth2 \
    &>/dev/null &

sleep 5

sudo ptf --test-dir ptf_nn/ptf_nn_test \
    --device-socket 0-{0-64}@tcp://127.0.0.1:10001 \
    --device-socket 1-{0-64}@tcp://127.0.0.1:10002 \
    --platform nn
