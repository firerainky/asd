# set(PUBLIC_MATH_HEADERS integer_interface.h big_integer.h native_integer.h)

# target_include_
aux_source_directory(. MATH_LIB_SRCS)

# add_library (MathFunctions ${MATH_LIB_SRCS})
add_library (MathFunctions core/big_integer.cpp)

set(MATH_INCLUDES ./core/)
set(MATH_INTERNAL ./internal/)
target_include_directories(MathFunctions INTERFACE ${MATH_INCLUDES} PRIVATE ${MATH_INTERNAL})

MESSAGE("math lib done !")

# reference： https://stackoverflow.com/questions/35385908/cmake-how-to-make-private-headers-of-a-library-available-to-its-tests