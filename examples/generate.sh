#!/usr/bin/env bash

HOLGEN_BIN=../cmake-build-debug/apps/cli/cli

$HOLGEN_BIN --input 1/schemas --cpp_output 1/output --namespace ex1_schemas --project ex1_lib --header ../../ExampleErrorConfig.h --lua_root 1/lua_gen --json
$HOLGEN_BIN --input 2/schemas --cpp_output 2/output --namespace ex2_schemas --project ex2_lib --header ../../ExampleErrorConfig.h --lua_root 2/lua_gen --json
$HOLGEN_BIN --input 3/schemas --cpp_output 3/output --namespace ex3_schemas --project ex3_lib --header ../../ExampleErrorConfig.h --lua_root 3/lua_gen --json
$HOLGEN_BIN --input 4/schemas --cpp_output 4/output --namespace ex4_schemas --project ex4_lib --header ../../ExampleErrorConfig.h --lua_root 4/lua_gen --json
