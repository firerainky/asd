set(TEST_MAIN unittests)
set(TEST_SOURCES
    big_integer_general_tests.cpp
    big_integer_mod_test.cpp
    main_tests.cpp
    vector_test.cpp
    numth_util_test.cpp
    ploy_test.cpp
    nbtheory_test.cpp
    fft_test.cpp)
set(TEST_INCLUDES ./)

add_executable(${TEST_MAIN} ${TEST_SOURCES})
target_link_libraries(${TEST_MAIN} PRIVATE MathFunctions GTest::GTest)
