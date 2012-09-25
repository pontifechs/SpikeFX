# Build the geo subdirectory
include_directories(${CMAKE_SOURCE_DIR}/src)

# Glob up all of the files
file(GLOB src ${CMAKE_SOURCE_DIR}/src/geo/*.*pp)
add_library(geo STATIC ${src})
set(GEO_LIBRARY geo)



