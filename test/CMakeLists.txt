find_package (Boost COMPONENTS program_options unit_test_framework log REQUIRED)
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

configure_file(files/test_addresses.csv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

foreach(t ${TESTS})
  add_executable (${t} ${t}.cpp)
  target_link_libraries (${t} i3ds i3ds_emulators i3ds_clients)
  add_test(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t})
endforeach(t)

find_program (BASH_PROGRAM bash)
if (BASH_PROGRAM)
  # To make it possible to re-run the test manually later, copy the script into the build-folder
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_recording.sh
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    FILE_PERMISSIONS USE_SOURCE_PERMISSIONS)
  add_test (recording_test ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test_recording.sh)
endif (BASH_PROGRAM)
