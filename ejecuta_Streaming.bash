#!/bin/bash

make -f Makefile_Streaming

./ServicioStreaming

make -f Makefile_Streaming clean

exit 0
