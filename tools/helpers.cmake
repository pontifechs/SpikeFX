

# Config

# Coverage flags
set(CMAKE_CXX_FLAGS_COVERAGE "-g -O0 -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS_COVERAGE "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
set(CMAKE_LD_FLAGS_COVERAGE "-fprofile-arcs -ftest-coverage")

# Add standard warning stuff
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")


# Functions
function(make_shared_library name "src_list")

  # Create the library
  add_library(${name} SHARED ${src_list})
    
  # Set up shared library versioning (consider using SOVERSION in the future)
  # and extra compile flags
  set_target_properties(${name} PROPERTIES
    COMPILE_FLAGS "-DCLF_EXPORTING")

  # Set up install parameters
  install(TARGETS ${name}
          DESTINATION lib/${CMAKE_BUILD_TYPE})

endfunction()

# Helper function to make a static library
function(make_static_library name "src_list")
# Create the library
  add_library(${name} STATIC ${src_list})

  # Add extra compile flags
  set_target_properties(
    ${name} PROPERTIES
    COMPILE_FLAGS "-DCLF_EXPORTING")

  # Set up install parameters
  install(TARGETS ${name}
          DESTINATION lib/${CMAKE_BUILD_TYPE})

endfunction()


# Helper function to make a test executable for a given library
function(make_test test_name lib_name)

  # Find all files that match *.*pp
  file(GLOB_RECURSE SRC_LIST ${GFX_SOURCE_DIR}/src/*/*.*pp)
  
  #foreach(f ${SRC_LIST})
  #  message(all ${f})
  #endforeach()
  
  # Get test files
  foreach(f ${SRC_LIST})
    message( ${f})
    if(f MATCHES ".*_test.cpp")
      message(match ${f})
      list(APPEND TEST_SRC_LIST ${f})
    endif()
  endforeach()

  # Create test executable
  list(APPEND TEST_SRC_LIST ${GTEST_ROOT}/src/gtest_main.cc)
  foreach(f ${TEST_SRC_LIST})
    message(test ${f})
  endforeach()

  add_executable(${test_name} ${TEST_SRC_LIST})

  # Link the appropriate libraries with the test executable
  target_link_libraries(${test_name} gtest ${lib_name} ${LD_EXTRA})
endfunction()
