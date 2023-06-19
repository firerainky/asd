aux_source_directory(math MATH_LIB_SRCS)

add_library (MathFunctions ${MATH_LIB_SRCS})
target_include_directories(MathFunctions PUBLIC math util)

MESSAGE(STATUS "math lib done !")