#!/bin/sh

cmake ..
make
./my_compiler ../testtext/test3.txt
./create_exec.sh
