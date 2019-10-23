#!/bin/bash

docker pull agodio/itba-so:1.0


docker run --user $(id -u):$(id -g) -v ${PWD}:/mnt agodio/itba-so:1.0 /bin/bash -c "cd /mnt; cd Toolchain && make all && cd .. && make clean && make debugger"

./runWithDebugger.sh