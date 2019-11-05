find_package (Boost COMPONENTS program_options unit_test_framework log REQUIRED)

set (SRCS
  client.cpp
  sensor_client.cpp
  client_factory.cpp
  camera_client.cpp
  tof_camera_client.cpp
  imu_client.cpp
  radar_client.cpp
  lidar_client.cpp
  analog_client.cpp
  star_tracker_client.cpp
  trigger_client.cpp
  power_client.cpp
  flash_client.cpp
  pose_estimator_client.cpp
  )

set (LIBS ${Boost_LIBRARIES} ${OpenCV_LIBS})

add_library (i3ds_clients SHARED ${SRCS})

target_link_libraries(i3ds_clients ${LIBS})

set_target_properties (
  i3ds_clients PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

target_compile_definitions(i3ds_clients PRIVATE)

install (TARGETS i3ds_clients DESTINATION lib)
install (DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
