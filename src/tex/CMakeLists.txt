# Build the tex subdirectory
include_directories(${CMAKE_SOURCE_DIR}/src)

# Glob up all of the files
file(GLOB src ${CMAKE_SOURCE_DIR}/src/tex/*.*pp)

add_library(tex STATIC ${src})
