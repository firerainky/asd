aux_source_directory(math MATH_LIB_SRCS)
aux_source_directory(util UTIL_LIB_SRCS)

add_library (MathFunctions ${MATH_LIB_SRCS} ${UTIL_LIB_SRCS})
target_include_directories(MathFunctions PUBLIC math util)
target_compile_options(MathFunctions PRIVATE -Werror)

MESSAGE(STATUS "math lib done !")