find_package (Boost COMPONENTS program_options REQUIRED)

add_executable (i3ds_address_server i3ds_address_server.cpp)
target_link_libraries (i3ds_address_server zmq pthread i3ds-utils ${Boost_LIBRARIES})

add_executable (i3ds_configure_camera i3ds_configure_camera.cpp)
target_link_libraries (i3ds_configure_camera i3ds-sensors)

install (TARGETS i3ds_address_server DESTINATION bin)
install (TARGETS i3ds_configure_camera DESTINATION bin)
