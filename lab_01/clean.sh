#!/bin/bash

SCRIPTPATH=$(dirname "$(readlink -f "$0")")

rm -f "$SCRIPTPATH"/*.o
rm -f "$SCRIPTPATH"/*.exe
rm -f "$SCRIPTPATH"/*.gc*
