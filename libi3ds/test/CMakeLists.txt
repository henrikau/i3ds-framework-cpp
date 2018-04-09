find_package(Boost 1.58 COMPONENTS unit_test_framework REQUIRED)

# FIXME: Include the generated headers.
include_directories ("${CMAKE_CURRENT_BINARY_DIR}/../src")
include_directories ("../emulator")

set(TESTS
  communication-test
  sensor-test
  camera-test
)

set(LIBS
  i3ds
  i3ds-emulator
  pthread
  ${Boost_LIBRARIES})

foreach(t ${TESTS})
  add_executable (${t} ${t}.cpp)
  target_link_libraries (${t} ${LIBS})
  add_test(${t} ${CMAKE_CURRENT_BINARY_DIR}/${t})
endforeach(t)
