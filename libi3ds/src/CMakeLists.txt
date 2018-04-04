include_directories ("${CMAKE_CURRENT_BINARY_DIR}")

find_package (ASN1CC REQUIRED)

asn1cc_generate_c (ASN1_SRCS ASN1_HDRS ${ASN1_FILES})

set (SRCS
  communication.cpp
  receiver.cpp
  client.cpp
  server.cpp
  publisher.cpp
  subscriber.cpp
  sensor.cpp
  camera.cpp
  ${ASN1_SRCS} ${ASN1_HDRS})

set (LIBS zmq)

add_library (i3ds SHARED ${SRCS})
target_link_libraries (i3ds ${LIBS})

set_target_properties (
  i3ds PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION})

install(TARGETS i3ds DESTINATION lib)
