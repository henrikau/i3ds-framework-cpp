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
  target_link_libraries (${t} i3ds i3ds_emulators)
  add_test(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t})
endforeach(t)

find_program (BASH_PROGRAM bash)
if (BASH_PROGRAM)
  add_test (recording_test ${BASH_PROGRAM} ${CMAKE_CURRENT_SOURCE_DIR}/test_recording.sh)
endif (BASH_PROGRAM)
