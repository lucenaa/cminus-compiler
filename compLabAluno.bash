#!/bin/bash

# Compile with parser enabled
cmake .. -DDOPARSE=TRUE
make

# Run tests on all example files
make runmycmcomp

# Generate detailed diffs comparing with reference output
make ddiff
