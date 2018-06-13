find_package (Boost COMPONENTS program_options unit_test_framework log REQUIRED)
find_package (ASN1CC REQUIRED)
find_package (OpenCV REQUIRED)

################################################################################
# ASN.1 generation
################################################################################

set (ESROCOS_BASE_DIR ${PROJECT_SOURCE_DIR}/asn/esrocos/types-base/asn)
set (I3DS_BASE_DIR ${PROJECT_SOURCE_DIR}/asn/i3ds/types-base/asn)
set (I3DS_BASE_DIR ${PROJECT_SOURCE_DIR}/asn/i3ds/types-base/asn)
set (I3DS_EXTRA_DIR ${PROJECT_SOURCE_DIR}/asn/i3ds/types-extra/asn)

set (ASN1_FILES
${ESROCOS_BASE_DIR}/Eigen.asn
${ESROCOS_BASE_DIR}/Point.asn
${ESROCOS_BASE_DIR}/taste-extended.asn
${ESROCOS_BASE_DIR}/taste-types.asn
${ESROCOS_BASE_DIR}/Temperature.asn
${ESROCOS_BASE_DIR}/Time.asn
${I3DS_BASE_DIR}/Analog.asn
${I3DS_BASE_DIR}/Common.asn
${I3DS_BASE_DIR}/DepthMap.asn
${I3DS_BASE_DIR}/IMU.asn
${I3DS_BASE_DIR}/PointCloud.asn
${I3DS_BASE_DIR}/Sensor.asn
${I3DS_BASE_DIR}/Camera.asn
${I3DS_BASE_DIR}/Frame.asn
${I3DS_BASE_DIR}/LIDAR.asn
${I3DS_BASE_DIR}/Radar.asn
${I3DS_BASE_DIR}/Region.asn
${I3DS_BASE_DIR}/SampleAttribute.asn
${I3DS_BASE_DIR}/StarTracker.asn
${I3DS_BASE_DIR}/ToFCamera.asn
${I3DS_EXTRA_DIR}/Trigger.asn
)

set (ASN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/i3ds")

asn1cc_generate_c (ASN1_SRCS ASN1_HDRS "${ASN_OUTPUT_DIR}" ${ASN1_FILES})

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
  src/radar_sensor.cpp
  src/radar_client.cpp
  src/emulated_lidar.cpp
  src/lidar_sensor.cpp
  src/lidar_client.cpp
  src/emulated_star_tracker.cpp
  src/star_tracker_client.cpp
  src/analog_client.cpp
  src/trigger.cpp
  src/trigger_client.cpp
  src/emulator_factory.cpp
  src/emulated_camera.cpp
  src/emulated_imu.cpp
  src/emulated_tof_camera.cpp
  src/emulated_radar.cpp
  src/emulated_tactile.cpp
  src/measurement_proxy.cpp
  )

set (LIBS
  zmq
  pthread
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
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
  lidar-test
  star-tracker-test
  tactile-test
  measurement-proxy-test
)

configure_file(test/test_addresses.csv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

foreach(t ${TESTS})
  add_executable (${t} test/${t}.cpp)
  target_link_libraries (${t} i3ds)
  add_test(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t})
endforeach(t)
