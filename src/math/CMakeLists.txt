# Build the math subdirectory
include_directories(${CMAKE_SOURCE_DIR}/src)

# Glob up all of the files
file(GLOB src ${CMAKE_SOURCE_DIR}/src/math/*.*pp)
add_library(math STATIC ${src})
set(MATH_LIBRARY math)


