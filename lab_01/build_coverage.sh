#!/bin/bash

SCRIPTPATH=$(dirname "$(readlink -f "$0")")

for src in "$SCRIPTPATH"/*.c; do
    no_ext=$(basename "$src" ".c")
    gcc -std=c99 -Wall -Werror -Wpedantic -Wextra -Wfloat-conversion -Wfloat-equal -c "$src" -o "$SCRIPTPATH"/"$no_ext".o --coverage -g
    compilation_status=$?
    if [ ! $compilation_status ]; then
        break
    fi
done

gcc -o "$SCRIPTPATH"/app.exe "$SCRIPTPATH"/*.o -lm --coverage
linking_status=$?

if [[ $compilation_status == 1 || $linking_status == 1 ]]; then
    exit 1
fi
