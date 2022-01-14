#!/bin/bash

make -f Makefile_Streaming

./ServicioStreaming $1

make -f Makefile_Streaming clean

exit 0