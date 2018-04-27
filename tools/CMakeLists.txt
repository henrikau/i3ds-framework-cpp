find_package (Boost COMPONENTS program_options log REQUIRED)

set (SRCS
  )

set (LIBS
  ${Boost_LIBRARIES}
  i3ds
  zmq
  pthread
  )

set (APPS
  i3ds_configure_camera
  )

foreach(a ${APPS})
  add_executable (${a} ${a}.cpp ${SRCS})
  target_link_libraries (${a} ${LIBS})
  install (TARGETS ${a} DESTINATION bin)
endforeach(t)
