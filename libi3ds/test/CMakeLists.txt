find_package(Boost 1.58 COMPONENTS unit_test_framework REQUIRED)

set(TESTS
  address-server-test
  camera-test
  communication-test
  sensor-test
)

set(LIBS
  i3ds-core
  i3ds-emulator
  i3ds-sensors
  i3ds-utils
  pthread
  zmq
  ${Boost_LIBRARIES})

configure_file(test_addresses.csv ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

foreach(t ${TESTS})
  add_executable (${t} ${t}.cpp)
  target_link_libraries (${t} ${LIBS})
  add_test(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t})
endforeach(t)
