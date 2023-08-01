set(UNIT_TEST_NAME unittests)
set(UNIT_TEST_SOURCES
    big_integer_general_tests.cpp
    big_integer_mod_test.cpp
    main_tests.cpp
    vector_test.cpp
    numth_util_test.cpp
    ploy_test.cpp
    nbtheory_test.cpp
    fft_test.cpp whole_process/encrypt_params_test.cpp)
set(UNIT_TEST_INCLUDES ./)

add_executable(${UNIT_TEST_NAME} ${UNIT_TEST_SOURCES})
target_link_libraries(${UNIT_TEST_NAME} PRIVATE MathFunctions GTest::GTest)

if(ENABLE_COVERAGE)
        set(COVERAGE_MAIN "coverage")
        set(COVERAGE_EXCLUDES
            "${PROJECT_SOURCE_DIR}/main.cpp"
            "${PROJECT_SOURCE_DIR}/cmake/*"
            "${PROJECT_SOURCE_DIR}/docs/*"
            "${PROJECT_SOURCE_DIR}/external/*"
            "${PROJECT_SOURCE_DIR}/test/*"
            "${PROJECT_SOURCE_DIR}/benchmark/*"
            "${PROJECT_SOURCE_DIR}/build/*"
            "/usr/include/*")
        setup_target_for_coverage_lcov(
            NAME
            ${COVERAGE_MAIN}
            EXECUTABLE
            ${UNIT_TEST_NAME}
            DEPENDENCIES
            ${UNIT_TEST_NAME})
    endif()
