include(FindSWIG)
include(UseSWIG)
include(FindPythonInterp)
include(FindPythonLibs)

if (SWIG_FOUND)

  set (SWIG_FLAGS "-I${PROJECT_SOURCE_DIR}/libi3ds/include")

  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set (SWIG_FLAGS "-DSWIGWORDSIZE64 ${SWIG_FLAGS}")
  endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

  if (PYTHONLIBS_FOUND)

    set (PYTHON_SWIG_SRC ${CMAKE_CURRENT_SOURCE_DIR}/python.i)

    set_property(SOURCE ${PYTHON_SWIG_SRC} PROPERTY CPLUSPLUS ON) 
    set_property(SOURCE ${PYTHON_SWIG_SRC} PROPERTY SWIG_MODULE_NAME i3ds)
    set_property(SOURCE ${PYTHON_SWIG_SRC} PROPERTY SWIG_FLAGS "${SWIG_FLAGS}")

    set(SWIG_MODULE_i3ds_python_EXTRA_DEPS "${CMAKE_CURRENT_SOURCE_DIR}/i3ds.i")

    swig_add_library(i3ds_python LANGUAGE python SOURCES ${PYTHON_SWIG_SRC})
    swig_link_libraries(i3ds_python i3ds ${PYTHON_LIBRARIES})

    set_source_files_properties(${swig_generated_file_fullname}
      PROPERTIES COMPILE_FLAGS "-fno-strict-aliasing -I${PYTHON_INCLUDE_PATH} -I${PROJECT_SOURCE_DIR}/include -w -O2")

    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
      OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

    install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -m py_compile i3ds.py
                              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})")

    install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} -O -m py_compile i3ds.py
                              WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})")

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/i3ds.py
      ${CMAKE_CURRENT_BINARY_DIR}/i3ds.pyc
      ${CMAKE_CURRENT_BINARY_DIR}/i3ds.pyo
      DESTINATION ${PYTHON_SITE_PACKAGES}
      )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/_i3ds_python.so
      RENAME _i3ds.so
      DESTINATION ${PYTHON_SITE_PACKAGES}
      )
    
  endif (PYTHONLIBS_FOUND)

endif (SWIG_FOUND)
