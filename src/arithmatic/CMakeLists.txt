set(ARITHMATIC_LIB arithmatic)
set(ARITHMATIC_SOURCES
    bb_integer.cpp
)
# set(ARITHMATIC_INCLUDES ./)

add_library (ARITHMATIC_LIB ${ARITHMATIC_SOURCES})
target_include_directories(ARITHMATIC_LIB PUBLIC ./)