# Need to have unit_test_framework available here for ../test/ to link properly..
if (BUILD_TESTS OR BUILD_TOOLS)
  find_package (Boost COMPONENTS program_options unit_test_framework log REQUIRED)
endif (BUILD_TESTS OR BUILD_TOOLS)

file(GLOB ASN1_SRCS ../generated/i3ds/*.c)
file(GLOB ASN1_HDRS ../generated/i3ds/*.h)

################################################################################
# libi3ds
################################################################################

set (SRCS
  ${ASN1_SRCS} ${ASN1_HDRS}
  src/communication.cpp
  src/address_server.cpp
  src/receiver.cpp
  src/server.cpp
  src/publisher.cpp
  src/subscriber.cpp
  src/periodic.cpp
  src/sensor.cpp
  src/camera_sensor.cpp
  src/gige_camera_sensor.cpp
  src/tof_camera_sensor.cpp
  src/radar_sensor.cpp
  src/lidar_sensor.cpp
  src/trigger.cpp
  src/power.cpp
  src/flash.cpp
  src/measurement_proxy.cpp
  src/configurator.cpp
  src/message_recording.cpp
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
