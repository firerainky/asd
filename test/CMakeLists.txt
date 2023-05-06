set(TEST_MAIN unittests)
set(TEST_SOURCES
  big_integer_tests.cpp
  main_tests.cpp
)
set(TEST_INCLUDES ./)

add_executable(${TEST_MAIN} ${TEST_SOURCES})
target_include_directories(${TEST_MAIN} PUBLIC ${TEST_INCLUDES})
target_link_libraries(${TEST_MAIN} PRIVATE MathFunctions GTest::GTest)