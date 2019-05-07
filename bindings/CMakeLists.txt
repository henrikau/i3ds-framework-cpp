include(FindSWIG)
include(UseSWIG)

find_package (Python3 COMPONENTS Interpreter Development)

if (SWIG_FOUND)

  set (SWIG_FLAGS "-py3 -I${PROJECT_SOURCE_DIR}/libi3ds/include")

  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set (SWIG_FLAGS "-DSWIGWORDSIZE64 ${SWIG_FLAGS}")
  endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

  if (Python3_FOUND)

    set (PYTHON_SWIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/python.i)

    set_source_files_properties(${PYTHON_SWIG_SRC} PROPERTIES CPLUSPLUS ON)
    set_source_files_properties(${PYTHON_SWIG_SRC} PROPERTIES SWIG_FLAGS "${SWIG_FLAGS}")
    set_source_files_properties(${PYTHON_SWIG_SRC} PROPERTIES SWIG_MODULE_NAME i3ds_binding)

    set(SWIG_MODULE_i3ds_python_EXTRA_DEPS "${CMAKE_CURRENT_SOURCE_DIR}/i3ds.i")

    swig_add_module(i3ds_python python ${PYTHON_SWIG_SRC})
    swig_link_libraries(i3ds_python i3ds ${PYTHON_LIBRARIES})

    set_source_files_properties(${swig_generated_file_fullname}
      PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing -I${Python3_INCLUDE_DIRS} -I${PROJECT_SOURCE_DIR}/include -w -O2")

    install(CODE "execute_process(COMMAND ${Python3_EXECUTABLE} -m py_compile i3ds_binding.py
                              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})")

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/i3ds_binding.py
      ${CMAKE_CURRENT_BINARY_DIR}/__pycache__/i3ds_binding.cpython-36.pyc
      DESTINATION ${Python3_SITELIB}
      )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/_i3ds_python.so
      RENAME _i3ds_binding.so
      DESTINATION ${Python3_SITELIB}
      )
    
  endif (Python3_FOUND)

endif (SWIG_FOUND)
