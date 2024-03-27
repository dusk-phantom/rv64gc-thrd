#!/bin/bash

test_target=$1
# if [ ! -f $test_target ]; then
#     echo "File not found!"
#     exit 1
# fi

echo "Building ${test_target}..."
gcc ./test/${test_target}.c ./libmy_thrd.so -o ${test_target}
echo "Running ${test_target}..."
./${test_target}
echo $?

