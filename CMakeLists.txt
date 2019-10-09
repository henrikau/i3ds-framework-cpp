cmake_minimum_required(VERSION 2.8)

################################################################################
# Set some project wide defintions
################################################################################

project (i3ds-framework-cpp)

set (PROJECT_AUTHOR "SINTEF Digital")
set (PROJECT_URL "http://i3ds-h2020.eu/")
set (PROJECT_LICENSE "MPLv2")
set (PROJECT_VERSION "1.0")

set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts)

option(BUILD_EMBEDDED "Build for embedded systems" OFF)
option(BUILD_TOOLS    "Build services" ON)
option(BUILD_TESTS    "Build tests and emulators" ON)
option(BUILD_DEBUG    "Build with debug enabled" OFF)
option(BUILD_BINDINGS "Build bindings" ON)
option(GENERATE_ASN   "Generate ASN files (instead of using pregenerated)" ON)
option(NO_OPENCV      "Force OpenCV detection off" OFF)

if (BUILD_EMBEDDED)
  if(NOT DEFINED CMAKE_FIND_ROOT_PATH)
    MESSAGE(FATAL_ERROR "CMAKE_FIND_ROOT_PATH path must be set when compiling for embedded")
  endif(NOT DEFINED CMAKE_FIND_ROOT_PATH)
  SET(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
  SET(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
endif (BUILD_EMBEDDED)

include_directories("external/include")

enable_testing ()

################################################################################
# Check and set C++ compiler flags
################################################################################

include (CheckCXXCompilerFlag REQUIRED)

function(set_cxx_flag FLAG)
  check_cxx_compiler_flag (${FLAG} CXX_SUPPORT_FLAG)
  if (${CXX_SUPPORT_FLAG})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}" PARENT_SCOPE)
  else ()
    message(FATAL_ERROR "Failed setting ${FLAG}, please update CMakeLists")
  endif ()
endfunction(set_cxx_flag)

if (NOT CMAKE_CXX_FLAGS)
  if (${BUILD_DEBUG})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set_cxx_flag("-ggdb3")
    set_cxx_flag("-Wall")
    set_cxx_flag("-pedantic")
    set_cxx_flag("-Wextra")
    set_cxx_flag("-Werror")
    set_cxx_flag("-Wfatal-errors")
  else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif ()

  set_cxx_flag("-std=c++17")

  check_cxx_compiler_flag ("-fpic" CXX_SUPPORT_FPIC_1)
  check_cxx_compiler_flag ("-fPIC" CXX_SUPPORT_FPIC_2)

  if (${CXX_SUPPORT_FPIC_1})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpic")
  elseif (${CXX_SUPPORT_FPIC_2})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  else ()
    message (WARNING "${CMAKE_CXX_COMPILER} does not support -fpic or -fPIC flags")
  endif ()

endif (NOT CMAKE_CXX_FLAGS)

################################################################################
# Check and set C compiler flags
################################################################################

include (CheckCCompilerFlag REQUIRED)

if (NOT CMAKE_C_FLAGS)

  if (${BUILD_DEBUG})
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wpedantic -Wall -Werror -Wextra")
    # catch most of what is undefined behavior
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wduplicated-cond -Wduplicated-branches")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmisleading-indentation -Wshadow")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-variadic-macros  -Wcast-align ")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-qual -Wstrict-aliasing=2")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wframe-larger-than=32768 -Wno-strict-overflow")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wsync-nand -Wtrampolines -Wsign-compare")

    # Will trigger a *lot* of warnings in generated asn.1
    # set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=float-equal")

    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=missing-braces")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=init-self -Werror=logical-op -Werror=write-strings")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=address -Werror=array-bounds -Werror=char-subscripts")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=enum-compare -Werror=implicit-int -Werror=empty-body")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=main -Werror=aggressive-loop-optimizations")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=nonnull -Werror=parentheses -Werror=pointer-sign")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=return-type -Werror=sequence-point -Werror=uninitialized")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=volatile-register-var -Werror=ignored-qualifiers")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=missing-parameter-type -Werror=old-style-declaration")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=maybe-uninitialized -Wno-unused-function -Wodr")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-signedness -Wsuggest-final-types -Wsuggest-final-methods")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-ignored-attributes -Wno-missing-field-initializers")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wshift-overflow=2 -Wduplicated-cond ")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wduplicated-branches -Werror=restrict")

    # Triggers in generated asn.1
    # set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wdouble-promotion")

    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat=2")
  else ()
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
  endif ()

  check_c_compiler_flag ("-Wall" C_SUPPORT_WALL)

  if (${C_SUPPORT_WALL})
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  endif ()

  check_c_compiler_flag ("-fpic" C_SUPPORT_FPIC_1)
  check_c_compiler_flag ("-fPIC" C_SUPPORT_FPIC_2)

  if (${C_SUPPORT_FPIC_1})
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
  elseif (${C_SUPPORT_FPIC_2})
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  else ()
    message (WARNING "${CMAKE_C_COMPILER} does not support -fpic or -fPIC flags")
  endif ()

endif (NOT CMAKE_C_FLAGS)

################################################################################
# Add subdirectories with sources of the framework
################################################################################

if (NO_OPENCV)
  set(OpenCV_FOUND 0)
else()
  find_package (OpenCV 3.0.0 QUIET)
endif()

include_directories (BEFORE "libi3ds/include/")
include_directories (BEFORE "generated")

if (BUILD_TESTS OR BUILD_TOOLS)
  include_directories (BEFORE "emulators/include")
  add_subdirectory(emulators)
endif (BUILD_TESTS OR BUILD_TOOLS)

add_subdirectory (libi3ds)

if (BUILD_TOOLS)
  add_subdirectory (services)
  add_subdirectory (tools)
endif (BUILD_TOOLS)

if (BUILD_BINDINGS)
  add_subdirectory (bindings)
endif (BUILD_BINDINGS)

##############################################################################
# Unit testing
##############################################################################
if (BUILD_TESTS)
  add_subdirectory (test)
endif (BUILD_TESTS)
