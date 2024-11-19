#!/bin/bash

SCRIPTPATH=$(dirname "$(readlink -f "$0")")

for src in "$SCRIPTPATH"/*.c; do
    no_ext=$(basename "$src" ".c")
    gcc -std=c99 -Wall -Werror -Wpedantic -Wextra -Wvla -Wfloat-conversion -Wfloat-equal -c "$src" -o "$SCRIPTPATH"/"$no_ext".o
done
gcc -o "$SCRIPTPATH"/app.exe "$SCRIPTPATH"/*.o -lm