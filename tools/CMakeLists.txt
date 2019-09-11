find_package (Boost COMPONENTS program_options log REQUIRED)

set (SRCS
  )

set (LIBS
  ${Boost_LIBRARIES}
  i3ds
  i3ds_emulators
  zmq
  pthread
  ${OpenCV_LIBS}
  )

set (APPS
  i3ds_configure_camera
  i3ds_configure_sensor
  i3ds_configure_lidar
  i3ds_configure_radar
  i3ds_configure_tof
  i3ds_configure_trigger
  i3ds_configure_power
  i3ds_configure_flash
#  i3ds_delay_recorder
  i3ds_record
  i3ds_replay
  i3ds_codec_performance_test
  i3ds_lidar_capture
  i3ds_analog_capture
  i3ds_imu_capture
  )

if (OpenCV_FOUND)
    set (APPS ${APPS} i3ds_camera_capture)
endif()

foreach(a ${APPS})

  add_executable (${a} ${a}.cpp ${SRCS})
  target_link_libraries (${a} ${LIBS})
  target_compile_definitions(${a} PRIVATE CAMERA_EMULATORS=${OpenCV_FOUND})
  install (TARGETS ${a} DESTINATION bin)
endforeach(a)
