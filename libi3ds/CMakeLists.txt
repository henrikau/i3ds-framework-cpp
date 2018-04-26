include_directories ("include/")
include_directories ("${CMAKE_CURRENT_BINARY_DIR}/src/core")

add_subdirectory (src)
add_subdirectory (test)
add_subdirectory (utils)

install (DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
