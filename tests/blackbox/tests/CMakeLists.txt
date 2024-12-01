add_executable(blackbox_tests
        test_weapon.cpp
        test_race.cpp
        test_compositeid.cpp
        test_country.cpp
        test_lua.cpp
        test_gamedata.cpp
        test_enum.cpp
        test_ref.cpp
        test_container.cpp
        test_struct.cpp
        test_json.cpp
        test_variant.cpp
        test_bitmap.cpp
)
add_dependencies(blackbox_tests gtest gtest_main gmock holgen_blackbox_test_lib lua_lib)
target_link_libraries(blackbox_tests gtest gtest_main gmock holgen_blackbox_test_lib lua_lib)
