#!/bin/bash

make -f Makefile_Master_Worker

./MasterWorker $1 $2 $3 $4 $5

make -f Makefile_Master_Worker clean

exit 0