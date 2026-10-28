#!/bin/sh
meld --diff "tests/output.ref.txt" "tests/bin/cl_x86/output.txt" --diff "tests/bin/cl_x86/output.txt" "tests/bin/cl_x64/output.txt"
