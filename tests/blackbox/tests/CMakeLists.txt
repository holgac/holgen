add_executable(tests test_weapon.cpp test_race.cpp test_country.cpp test_lua.cpp test_gamedata.cpp test_enum.cpp test_ref.cpp test_container.cpp test_struct.cpp test_json.cpp test_variant.cpp test_bitmap.cpp)
add_dependencies(tests gtest gtest_main gmock holgen_blackbox_test_lib lua_lib)
target_link_libraries(tests gtest gtest_main gmock holgen_blackbox_test_lib lua_lib)
