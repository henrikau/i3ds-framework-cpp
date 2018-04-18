add_executable (i3ds_address_server i3ds_address_server.cpp)
target_link_libraries (i3ds_address_server zmq pthread i3ds-utils)

install (TARGETS i3ds_address_server DESTINATION bin)
