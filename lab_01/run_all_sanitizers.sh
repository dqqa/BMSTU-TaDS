#!/bin/bash

SCRIPTPATH=$(dirname "$(readlink -f "$0")")

all_done=true
for file in "$SCRIPTPATH"/build_debug_*.sh; do
    basename "$file"
    "$SCRIPTPATH"/clean.sh
    echo "Compiling..."
    "$file"
    if ! "$SCRIPTPATH"/func_tests/scripts/func_tests.sh; then
        all_done=false
    fi
done

if ! $all_done; then
    echo "There are some errors"
    exit 1
fi

echo "All sanitizers passed func testing"