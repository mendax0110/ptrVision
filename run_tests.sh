#!/bin/bash

EXE_PATH="./build_linux/ptrVision"
TEST_DIR="test"

echo "Running tests with ${EXE_PATH} using ${TEST_DIR} as the test directory..."
$EXE_PATH $TEST_DIR
