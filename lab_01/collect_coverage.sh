#!/bin/bash

SCRIPTPATH=$(dirname "$(readlink -f "$0")")

# Очистка всего мусора
echo "Cleaning trash files"
"$SCRIPTPATH"/clean.sh

# Компиляция с флагом покрытия --coverage
echo "Compiling..."

if ! "$SCRIPTPATH"/build_coverage.sh; then
    echo "Compilation error. Aborting tests"
    exit 1
else
    echo "Compilation done"
fi

if [ ! -d "$SCRIPTPATH"/func_tests ]; then
    echo "No func_tests dir found!"
    exit 1
fi

# Запуск скрипта функционального тестирования
"$SCRIPTPATH"/func_tests/scripts/func_tests.sh -v

# Теперь у нас есть вся необходимая информация, и можно ее обработать при помощи gcov
echo "Analysing collected data with gcov"
gcov -n -a "$SCRIPTPATH"/*.gcda | head -n -1
echo
