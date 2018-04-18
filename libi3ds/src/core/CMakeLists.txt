find_package (ASN1CC REQUIRED)

asn1cc_generate_c (ASN1_SRCS ASN1_HDRS ${ASN1_FILES})

include_directories (BEFORE "${CMAKE_CURRENT_BINARY_DIR}")

set (SRCS
  client.cpp
  communication.cpp
  publisher.cpp
  receiver.cpp
  server.cpp
  subscriber.cpp
  ${ASN1_SRCS} ${ASN1_HDRS})

set (LIBS zmq)

add_library (i3ds-core SHARED ${SRCS})
target_link_libraries (i3ds-core ${LIBS})

set_target_properties (
  i3ds-core PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

install(TARGETS i3ds-core DESTINATION lib)

install(FILES ${ASN1_HDRS} DESTINATION include/i3ds/core)
