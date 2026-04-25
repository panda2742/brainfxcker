#!/bin/sh

docker build --platform linux/amd64 -t brainfxcker-amd64 . && \
clear && \
# docker run --platform linux/amd64 brainfxcker-amd64 /bin/bash -c "./brainfxcker programs/main.bf ; ./a.out"
docker run --platform linux/amd64 -it brainfxcker-amd64 /bin/bash
