include_directories ("include/")

add_subdirectory (src)
add_subdirectory (emulator)
add_subdirectory (test)

install (DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
