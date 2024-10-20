#!/usr/bin/env bash

HOLGEN_BIN=../cmake-build-debug/apps/cli/cli

$HOLGEN_BIN --input 1/schemas --cpp_output 1/output --namespace ex1_schemas --cmake ex1_lib --header ../../ExampleErrorConfig.h --lua --json
$HOLGEN_BIN --input 2/schemas --cpp_output 2/output --namespace ex2_schemas --cmake ex2_lib --header ../../ExampleErrorConfig.h --lua --json
$HOLGEN_BIN --input 3/schemas --cpp_output 3/output --namespace ex3_schemas --cmake ex3_lib --header ../../ExampleErrorConfig.h --lua --json
$HOLGEN_BIN --input 4/schemas --cpp_output 4/output --namespace ex4_schemas --cmake ex4_lib --header ../../ExampleErrorConfig.h --lua --json
