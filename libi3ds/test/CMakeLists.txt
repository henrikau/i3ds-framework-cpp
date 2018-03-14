find_package(Boost 1.58 COMPONENTS unit_test_framework REQUIRED)

# FIXME: Include the generated headers.
include_directories ("${CMAKE_CURRENT_BINARY_DIR}/../src")

set(TESTS
  codec-test
  sensor-test
  zmq_msg_codec-test
)

set(LIBS
  i3ds
  zmq
  ${Boost_LIBRARIES})

foreach(t ${TESTS})
  add_executable (${t} ${t}.cpp)
  target_link_libraries (${t} ${LIBS})
  add_test(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t})
endforeach(t)
