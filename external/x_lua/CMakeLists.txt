set(CORE_O src/lapi.c src/lcode.c src/lctype.c src/ldebug.c src/ldo.c src/ldump.c src/lfunc.c src/lgc.c src/llex.c src/lmem.c src/lobject.c src/lopcodes.c src/lparser.c src/lstate.c src/lstring.c src/ltable.c src/ltm.c src/lundump.c src/lvm.c src/lzio.c src/ltests.c)
set(AUX_O src/lauxlib.c)
set(LIB_O src/lbaselib.c src/ldblib.c src/liolib.c src/lmathlib.c src/loslib.c src/ltablib.c src/lstrlib.c src/lutf8lib.c src/loadlib.c src/lcorolib.c src/linit.c)

add_library(lua_lib ${CORE_O} ${AUX_O} ${LIB_O})
add_executable(lua_main src/lua.c)
add_dependencies(lua_main lua_lib)
target_link_libraries(lua_main lua_lib)
