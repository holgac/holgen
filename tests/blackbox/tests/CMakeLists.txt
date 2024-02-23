add_executable(tests test_weapon.cpp test_race.cpp test_country.cpp test_lua.cpp test_gamedata.cpp)
add_dependencies(tests gtest gtest_main lua_lib holgen_blackbox_test_lib)
target_link_libraries(tests gtest gtest_main lua_lib holgen_blackbox_test_lib)
