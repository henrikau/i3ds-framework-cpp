cmake_minimum_required(VERSION 2.8)

################################################################################
# Set some project wide defintions
################################################################################

project (i3ds-interfaces)

set (PROJECT_AUTHOR "SINTEF Digital")
set (PROJECT_URL "http://i3ds-h2020.eu/")
set (PROJECT_LICENSE "MPLv2")
set (PROJECT_VERSION "1.0")

enable_testing ()

################################################################################
# Set paths common for whole framework
################################################################################

set (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/scripts)

################################################################################
# ASN.1 generation
################################################################################

set (ESROCOS_ASN1_DIR ${PROJECT_SOURCE_DIR}/esrocos/types-base/asn)
set (I3DS_ASN1_DIR ${PROJECT_SOURCE_DIR}/i3ds/types-base/asn)

set (ASN1_FILES
${ESROCOS_ASN1_DIR}/Eigen.asn
${ESROCOS_ASN1_DIR}/Point.asn
${ESROCOS_ASN1_DIR}/taste-extended.asn
${ESROCOS_ASN1_DIR}/taste-types.asn
${ESROCOS_ASN1_DIR}/Temperature.asn
${ESROCOS_ASN1_DIR}/Time.asn
${I3DS_ASN1_DIR}/Analog.asn
${I3DS_ASN1_DIR}/Common.asn
${I3DS_ASN1_DIR}/DepthMap.asn
${I3DS_ASN1_DIR}/IMU.asn
${I3DS_ASN1_DIR}/PointCloud.asn
${I3DS_ASN1_DIR}/Sensor.asn
${I3DS_ASN1_DIR}/Camera.asn
${I3DS_ASN1_DIR}/Frame.asn
${I3DS_ASN1_DIR}/LIDAR.asn
${I3DS_ASN1_DIR}/Radar.asn
${I3DS_ASN1_DIR}/Region.asn
${I3DS_ASN1_DIR}/SampleAttribute.asn
${I3DS_ASN1_DIR}/StarTracker.asn
${I3DS_ASN1_DIR}/ToFCamera.asn
)

################################################################################
# Check that the C++ compiler supports C++11 or C++0X, else issue warning.
################################################################################

include (CheckCXXCompilerFlag REQUIRED)

if (NOT CMAKE_CXX_FLAGS)

  set (DEBUG $ENV{DEBUG})

  if (${DEBUG})
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
  else ()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
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
# Add subdirectories with sources of the framework
################################################################################

add_subdirectory (libi3ds)
