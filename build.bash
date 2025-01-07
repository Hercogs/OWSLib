#!/bin/bash

# Get the script's full path
SCRIPT_PATH=$(realpath "${BASH_SOURCE[0]}")
# Get the script's directory
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")

# echo $SCRIPT_DIR
(cd $SCRIPT_DIR && mkdir -p build && cd build && cmake .. && make)
# # (mkdir -p build; cd build; cmake ..; make)





