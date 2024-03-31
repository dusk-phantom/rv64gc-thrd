#!/bin/bash

# 检查参数数量
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <test_name>"
    exit 1
fi

TEST_NAME=$1
TEST_FILE="test/${TEST_NAME}.c"
OUTPUT_FILE="build/test/${TEST_NAME}"

if [ ! -d "build/test" ]; then
    mkdir -p build/test
fi

# 检查测试文件是否存在
if [ ! -f "${TEST_FILE}" ]; then
    echo "Test file ${TEST_FILE} does not exist."
    exit 1
fi

# 编译测试文件，并链接到 libthrd.so
gcc -Wall -g -Iinc -o ${OUTPUT_FILE} ${TEST_FILE} -Lbuild -lthrd -Wl,-rpath,build

# 检查编译是否成功
if [ "$?" -ne 0 ]; then
    echo "Compilation failed."
    exit 1
else
    echo "Compilation successful. Output file: ${OUTPUT_FILE}"
fi

./${OUTPUT_FILE}

echo "main output is $?"


