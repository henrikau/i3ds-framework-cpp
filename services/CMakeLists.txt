find_package (Boost COMPONENTS program_options log REQUIRED)

set (SRCS
  )

set (LIBS
  ${Boost_LIBRARIES}
  i3ds
  i3ds_emulators
  zmq
  pthread
  )

set (APPS
  i3ds_suite_emulator
  i3ds_trigger_emulator
  i3ds_flash_emulator
  i3ds_address_server
  i3ds_measurement_proxy
  )

if (OpenCV_FOUND)
    set (APPS ${APPS} i3ds_camera_emulator)
endif()

foreach(a ${APPS})
  add_executable (${a} ${a}.cpp ${SRCS})
  target_link_libraries (${a} ${LIBS})
  target_compile_definitions(${a} PRIVATE CAMERA_EMULATORS=${OpenCV_FOUND})
  install (TARGETS ${a} DESTINATION bin)
endforeach(a)
