#!/bin/bash

make -f Makefile_Cliente_Analizador

./Cliente_Analizador $1 $2 

make -f Makefile_Cliente_Analizador clean

exit 0
