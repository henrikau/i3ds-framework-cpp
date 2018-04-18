set (SRCS
  address_server.cpp
)

set (LIBS zmq)

add_library (i3ds-utils SHARED ${SRCS})
target_link_libraries (i3ds-utils ${LIBS})

set_target_properties (
  i3ds-utils PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

install(TARGETS i3ds-utils DESTINATION lib)
