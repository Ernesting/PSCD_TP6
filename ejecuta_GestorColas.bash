#!/bin/bash

make -f Makefile_Gestor

./Gestor_Colas $1 $2 $3 

make -f Makefile_Gestor clean

exit 0
