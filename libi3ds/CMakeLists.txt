include_directories ("include/")

add_subdirectory (src)
add_subdirectory (test)
add_subdirectory (utils)

install (DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
