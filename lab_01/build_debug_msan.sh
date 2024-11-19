#!/bin/bash

SCRIPTPATH=$(dirname "$(readlink -f "$0")")

for src in "$SCRIPTPATH"/*.c; do
    no_ext=$(basename "$src" ".c")
    clang -std=c99 -Wall -Werror -Wpedantic -Wvla -Wextra -g -fno-omit-frame-pointer -fsanitize=memory -fPIE -c "$src" -o "$SCRIPTPATH"/"$no_ext".o
done
clang -fno-omit-frame-pointer -fsanitize=memory -pie -o "$SCRIPTPATH"/app.exe "$SCRIPTPATH"/*.o -lm