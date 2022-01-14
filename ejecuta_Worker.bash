#!/bin/bash

make -f Makefile_Worker

./Worker $1 $2 

make -f Makefile_Worker clean

exit 0
