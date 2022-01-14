#!/bin/bash

make -f Makefile_Master

./Master $1 $2 $3 $4

make -f Makefile_Master clean

exit 0
