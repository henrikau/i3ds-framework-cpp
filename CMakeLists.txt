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
set (DEBUG $ENV{DEBUG})

option(BUILD_EMBEDDED "Build for embedded systems" OFF)
option(BUILD_TOOLS    "Build services" ON)
option(BUILD_TESTS    "Build tests and emulators" ON)
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
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Werror -ggdb3")
endif (BUILD_EMBEDDED)

include_directories("external/include")

enable_testing ()

################################################################################
# Check and set C++ compiler flags
################################################################################

include (CheckCXXCompilerFlag REQUIRED)

if (NOT CMAKE_CXX_FLAGS)

  if (${DEBUG})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
  else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif ()

  check_cxx_compiler_flag ("-std=c++11" CXX_SUPPORT_STD_11)

  if (${CXX_SUPPORT_STD_11})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else ()
    message (WARNING "${CMAKE_CXX_COMPILER} does not support -std=c++11 flag")
  endif ()

  check_cxx_compiler_flag ("-Wall" CXX_SUPPORT_WALL)

  if (${CXX_SUPPORT_WALL})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  endif ()

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

  if (${DEBUG})
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
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

include_directories (BEFORE "libi3ds/include/")
include_directories (BEFORE "generated")

add_subdirectory (libi3ds)

if (BUILD_TOOLS)
  add_subdirectory (services)
  add_subdirectory (tools)
endif (BUILD_TOOLS)

if (BUILD_BINDINGS)
  add_subdirectory (bindings)
endif (BUILD_BINDINGS)
