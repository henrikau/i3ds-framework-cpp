if (BUILD_TESTS OR BUILD_TOOLS)
  find_package (Boost COMPONENTS program_options unit_test_framework log REQUIRED)

  if (NO_OPENCV)
    set(OpenCV_FOUND 0)
  else()
    find_package (OpenCV 3.0.0 QUIET)
  endif()

endif (BUILD_TESTS OR BUILD_TOOLS)

file(GLOB ASN1_SRCS ../generated/i3ds/*.c)
file(GLOB ASN1_HDRS ../generated/i3ds/*.h)

################################################################################
# libi3ds
################################################################################

set (LIB_SRCS
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
  src/gige_camera_sensor.cpp
  src/camera_client.cpp
  src/tof_camera_sensor.cpp
  src/tof_camera_client.cpp
  src/imu_client.cpp
  src/radar_sensor.cpp
  src/radar_client.cpp
  src/lidar_sensor.cpp
  src/lidar_client.cpp
  src/star_tracker_client.cpp
  src/analog_client.cpp
  src/trigger.cpp
  src/trigger_client.cpp
  src/power.cpp
  src/power_client.cpp
  src/flash.cpp
  src/flash_client.cpp
  src/measurement_proxy.cpp
  src/sensor_configurator.cpp
  src/message_recording.cpp
  )

set (EMULATOR_SRCS
  src/emulator_factory.cpp
  src/emulated_lidar.cpp
  src/emulated_star_tracker.cpp
  src/emulated_imu.cpp
  src/emulated_radar.cpp
  src/emulated_analog.cpp
  src/emulated_trigger.cpp
  src/emulated_flash.cpp
  )

set (CAMERA_EMULATOR_SRCS
  src/emulated_camera.cpp
  src/opencv_convertion.cpp
  src/emulated_tof_camera.cpp
  )

set (LIBS
  zmq
  pthread
  ${Boost_LIBRARIES}
  ${OpenCV_LIBS}
  )

set (SRCS ${LIB_SRCS} )

# Add emulators to sources if tests are built.
# Do not add camera emulators on embedded
if (BUILD_TESTS OR BUILD_TOOLS)
  if (OpenCV_FOUND)
    set (SRCS
      ${SRCS}
      ${EMULATOR_SRCS}
      ${CAMERA_EMULATOR_SRCS}
      )
  else (OpenCV_FOUND)
    set (SRCS
      ${SRCS}
      ${EMULATOR_SRCS}
      )
  endif (OpenCV_FOUND)
endif(BUILD_TESTS OR BUILD_TOOLS)

add_library (i3ds SHARED ${SRCS})

target_link_libraries (i3ds ${LIBS})

set_target_properties (
  i3ds PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

target_compile_definitions(i3ds PRIVATE CAMERA_EMULATORS=${OpenCV_FOUND})

install (TARGETS i3ds DESTINATION lib)
install (DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install (FILES ${ASN1_HDRS} DESTINATION include/i3ds)

################################################################################
# Unit testing
################################################################################

if (BUILD_TESTS)
  set(TESTS
    address-server-test
    communication-test
    imu-test
    sensor-test
    radar-test
    lidar-test
    star-tracker-test
    analog-test
    measurement-proxy-test
    pointcloud-test
    depthmap-test
    )

  if (OpenCV_FOUND)
    set(TESTS
      ${TESTS}
      camera-test
      tof-camera-test
      )
  endif (OpenCV_FOUND)

  configure_file(test/test_addresses.csv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

  foreach(t ${TESTS})
    add_executable (${t} test/${t}.cpp)
    target_link_libraries (${t} i3ds)
    add_test(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t})
  endforeach(t)

endif (BUILD_TESTS)
