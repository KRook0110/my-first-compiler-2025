#!/bin/sh

cmake -S . -B ./build/
make -C ./build/

./build/my_compiler $1
status=$?

if [ $status -ne 0 ]; then
    echo "Compiler Returned $status"
    exit 1
fi

nasm -f elf64 out.asm -o ./build/out.o
ld -s -o ./build/out ./build/out.o

./build/out
status=$?

echo "program returned $status"

