aux_source_directory(. MATH_LIB_SRCS)
aux_source_directory(internal MATH_LIB_SRCS_INTERNAL)
include_directories(../util)
# include_directories(internal/)

add_library (MathFunctions ${MATH_LIB_SRCS} ${MATH_LIB_SRCS_INTERNAL})
# target_include_directories(MathFunctions PRIVATE internal)
target_include_directories(MathFunctions PUBLIC ./)

target_sources(MathFunctions PRIVATE internal/basic_arithmatic.h)

MESSAGE("math lib done !")