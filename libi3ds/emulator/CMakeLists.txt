include_directories ("${CMAKE_CURRENT_BINARY_DIR}/../src")

set (SRCS
  periodic_sampler.cpp
  emulated_camera.cpp)

set (LIBS i3ds)

add_library (i3ds-emulator SHARED ${SRCS})
target_link_libraries (i3ds-emulator ${LIBS})

set_target_properties (
  i3ds-emulator PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

install(TARGETS i3ds-emulator DESTINATION lib)
