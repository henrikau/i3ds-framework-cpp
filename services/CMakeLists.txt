find_package (Boost COMPONENTS program_options log REQUIRED)

if (NO_OPENCV)
  set(OpenCV_FOUND 0)
else()
  find_package (OpenCV 3.0.0 QUIET)
endif()

set (SRCS
  )

set (LIBS
  ${Boost_LIBRARIES}
  i3ds
  zmq
  pthread
  )

set (APPS
  i3ds_suite_emulator
  i3ds_camera_emulator
  i3ds_trigger_emulator
  i3ds_flash_emulator
  i3ds_address_server
  i3ds_measurement_proxy
  )

foreach(a ${APPS})
  add_executable (${a} ${a}.cpp ${SRCS})
  target_link_libraries (${a} ${LIBS})
  target_compile_definitions(${a} PRIVATE CAMERA_EMULATORS=${OpenCV_FOUND})
  install (TARGETS ${a} DESTINATION bin)
endforeach(a)
