#!/bin/sh

docker build --platform linux/amd64 -t brainfxcker-amd64 . && clear && docker run --platform linux/amd64 brainfxcker-amd64 valgrind $VGFLAGS ./brainfxcker programs/main.bf
