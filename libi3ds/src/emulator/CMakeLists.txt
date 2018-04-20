set (SRCS
  emulated_camera.cpp
  emulated_tof_camera.cpp
  periodic_sampler.cpp
)

set (LIBS i3ds-sensors)

add_library (i3ds-emulator SHARED ${SRCS})
target_link_libraries (i3ds-emulator ${LIBS})

set_target_properties (
  i3ds-emulator PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

install(TARGETS i3ds-emulator DESTINATION lib)
