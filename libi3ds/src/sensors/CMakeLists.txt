set (SRCS
  camera.cpp
  camera_client.cpp
  sensor.cpp
)

set (LIBS i3ds-core)

add_library (i3ds-sensors SHARED ${SRCS})
target_link_libraries (i3ds-sensors ${LIBS})

set_target_properties (
  i3ds-sensors PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

install(TARGETS i3ds-sensors DESTINATION lib)
