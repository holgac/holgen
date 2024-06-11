#!/usr/bin/env bash

HOLGEN_BIN=../cmake-build-debug/apps/cli/cli

$HOLGEN_BIN 1/schemas 1/output ex1_schemas  ex1_lib ../../ExampleErrorConfig.h
$HOLGEN_BIN 2/schemas 2/output ex2_schemas  ex2_lib ../../ExampleErrorConfig.h
$HOLGEN_BIN 3/schemas 3/output ex3_schemas  ex3_lib ../../ExampleErrorConfig.h
$HOLGEN_BIN 4/schemas 4/output ex4_schemas  ex4_lib ../../ExampleErrorConfig.h
