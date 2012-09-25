project(Spike)
cmake_minimum_required(VERSION 2.8)


# Include custom helper function
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tools")

# Find External Libraries
find_package(GLUI REQUIRED)
#find_package(GLEW REQUIRED)
#FIND_PACKAGE( OpenCV REQUIRED )


# Build subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/src/math)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/geo)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/tex)

# Build Executable
include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories(${ImageMagick_INCLUDE_DIRS})
include_directories(${ImageMagick_Magick++_INCLUDE_DIRS})

add_executable(Spike ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Link Libraries
target_link_libraries(Spike 
  ${GLUI_LIBRARY} 
  ${GLEW_LIBRARY} 
  ${GLUT_LIBRARY} 
  ${OPENGL_LIBRARY}
#  ${OpenCV_LIBS}  
  math
  geo
  tex
  )