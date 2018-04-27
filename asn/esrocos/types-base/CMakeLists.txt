# ======================================================================
#  FILE:  $URL$
# ======================================================================
#  PROJECT:             :  SARGON
#  VERSION              :  $Revision$
#  LANGUAGE             :  CMake script
#  AUTHOR               :  $LastChangedBy$ 
#  COPYRIGHT            :  AVOS - GMV,S.A.
#  COMPILER             :  cmake version 3.0.2
#  CREATED              :  $CreationDate$
#  CLASS                :  -
#  LAST MODIFIED        :  $LastChangedDate$
#  GENERATED FROM MODEL :  -
#  ORIGINAL MODEL AUTHOR:  -
# ----------------------------------------------------------------------
#  Toplevel CMake build script for the asn1_types library
# ----------------------------------------------------------------------
#  HISTORY
#  $History$
# ======================================================================

# CMakeLists.txt has to be located in the project folder and cmake has to be
# executed from 'project/build' with 'cmake ../'.

cmake_minimum_required(VERSION 2.6)

project(types-base)

message(STATUS "Running install-asn-types.sh script")

set(GENERATED_AADL "${CMAKE_PROJECT_NAME}.aadl")

add_custom_command(OUTPUT ${GENERATED_AADL}
		   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		   COMMAND sh "-c" "${CMAKE_SOURCE_DIR}/scripts/install-asn-types.sh ${CMAKE_SOURCE_DIR}/asn ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_PROJECT_NAME}")

add_custom_target(asn2aadl ALL DEPENDS ${GENERATED_AADL})


install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_AADL}
	DESTINATION ${CMAKE_INSTALL_PREFIX}/types/${CMAKE_PROJECT_NAME})
