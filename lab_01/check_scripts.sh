#!/bin/bash

SCRIPTPATH=$(dirname "$(readlink -f "$0")")

echo "Scripts check"
all_good=1

for file in "$SCRIPTPATH"/*.sh "$SCRIPTPATH"/func_tests/scripts/*.sh; do
    if shellcheck "$file" > /dev/null;
    then
        echo "[OK] $file"
    else
        echo "[ERROR] $file"
        all_good=0
    fi
done

if [ $all_good -eq 0 ]; then
    echo "Not all scripts passed shellcheck"
    exit 1
else
    echo "All scripts passed shellcheck"
fi