find_package (Boost COMPONENTS program_options unit_test_framework log REQUIRED)

set (EMULATOR_SRCS
  emulator_factory.cpp
  emulated_lidar.cpp
  emulated_star_tracker.cpp
  emulated_imu.cpp
  emulated_radar.cpp
  emulated_analog.cpp
  emulated_trigger.cpp
  emulated_flash.cpp
  )

set (CAMERA_EMULATOR_SRCS
   opencv_convertion.cpp
   emulated_camera.cpp
   emulated_tof_camera.cpp
   )

# Do not add camera emulators on embedded
set (SRCS
  ${SRCS}
  ${EMULATOR_SRCS})
if (OpenCV_FOUND)
  set (SRCS
    ${SRCS}
    ${CAMERA_EMULATOR_SRCS}
    )
endif (OpenCV_FOUND)

set (LIBS ${Boost_LIBRARIES} ${OpenCV_LIBS})

add_library (i3ds_emulators SHARED ${SRCS})

target_link_libraries(i3ds_emulators ${LIBS})

set_target_properties (
  i3ds_emulators PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

target_compile_definitions(i3ds_emulators PRIVATE CAMERA_EMULATORS=${OpenCV_FOUND})

install (TARGETS i3ds_emulators DESTINATION lib)
install (DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
