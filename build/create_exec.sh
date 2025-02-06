#!/bin/sh


nasm -f elf64 out.asm -o out.o
ld out.o -o out



