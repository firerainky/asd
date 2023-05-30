set(TEST_MAIN unittests)
set(TEST_SOURCES
        big_integer_general_tests.cpp
        big_integer_mod_test.cpp
        main_tests.cpp
        vector_test.cpp)
set(TEST_INCLUDES ./)

add_executable(${TEST_MAIN} ${TEST_SOURCES})
target_include_directories(${TEST_MAIN} PUBLIC ${TEST_INCLUDES})
target_link_libraries(${TEST_MAIN} PRIVATE MathFunctions GTest::GTest)