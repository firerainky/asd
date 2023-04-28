aux_source_directory(. MATH_LIB_SRCS)
include_directories(../util)

add_library (MathFunctions ${MATH_LIB_SRCS})
target_include_directories(MathFunctions PUBLIC ./)

MESSAGE("math lib done !")