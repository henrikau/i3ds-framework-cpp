find_package (Boost COMPONENTS program_options log REQUIRED)
find_package (OpenCV REQUIRED)

set (SRCS
  )

set (LIBS
  ${Boost_LIBRARIES}
  i3ds
  zmq
  pthread
  ${OpenCV_LIBS}
  )

set (APPS
  i3ds_camera_capture
  i3ds_configure_camera
  i3ds_configure_sensor
#  i3ds_delay_recorder
  i3ds_record
  i3ds_replay
  )

foreach(a ${APPS})
  add_executable (${a} ${a}.cpp ${SRCS})
  target_link_libraries (${a} ${LIBS})
  install (TARGETS ${a} DESTINATION bin)
endforeach(a)
