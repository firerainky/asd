set(BENCHMARK_MAIN fhe_benchmark)
set(BENCHMARK_SOURCES big_integer_benchmark.cpp)
set(BENCHMARK_INCLUDES ./)

add_executable(${BENCHMARK_MAIN} ${BENCHMARK_SOURCES})
target_link_libraries(${BENCHMARK_MAIN} PRIVATE MathFunctions
                                                benchmark::benchmark)
