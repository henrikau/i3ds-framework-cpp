find_package (Boost COMPONENTS program_options unit_test_framework log REQUIRED)
find_package (ASN1CC REQUIRED)

################################################################################
# ASN.1 generation
################################################################################

set (ESROCOS_ASN1_DIR ${PROJECT_SOURCE_DIR}/asn/esrocos/types-base/asn)
set (I3DS_ASN1_DIR ${PROJECT_SOURCE_DIR}/asn/i3ds/types-base/asn)

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

asn1cc_generate_c (ASN1_SRCS ASN1_HDRS ${ASN1_FILES})

################################################################################
# libi3ds
################################################################################

set (SRCS
  ${ASN1_SRCS} ${ASN1_HDRS}
  src/communication.cpp
  src/address_server.cpp
  src/receiver.cpp
  src/client.cpp
  src/server.cpp
  src/publisher.cpp
  src/subscriber.cpp
  src/periodic.cpp
  src/sensor.cpp
  src/sensor_client.cpp
  src/client_factory.cpp
  src/camera_sensor.cpp
  src/camera_client.cpp
  src/tof_camera_sensor.cpp
  src/tof_camera_client.cpp
  src/imu_client.cpp
  src/emulator_factory.cpp
  src/emulated_camera.cpp
  src/emulated_imu.cpp
  src/emulated_tof_camera.cpp
  src/emulated_radar.cpp
  src/radar_sensor.cpp
  src/radar_client.cpp
  )

set (LIBS
  zmq
  ${Boost_LIBRARIES}
  )

add_library (i3ds SHARED ${SRCS})
target_link_libraries (i3ds ${LIBS})

set_target_properties (
  i3ds PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

install (TARGETS i3ds DESTINATION lib)
install (DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install (FILES ${ASN1_HDRS} DESTINATION include/i3ds)

################################################################################
# Unit testing
################################################################################

set(TESTS
  address-server-test
  camera-test
  communication-test
  imu-test
  sensor-test
  tof-camera-test
  radar-test
)

configure_file(test/test_addresses.csv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

foreach(t ${TESTS})
  add_executable (${t} test/${t}.cpp)
  target_link_libraries (${t} i3ds)
  add_test(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t})
endforeach(t)
