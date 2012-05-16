cmake_minimum_required(VERSION 2.6)

PROJECT(dynamind-urbansim)

FIND_PATH(DYNAMINDDEV DynaMindDevCmake.txt
            PATHS   $ENV{DYNAMIND_DIR}/include
            PATH_SUFFIXES dynamindcore/CMake)

IF(DYNAMINDDEV)
    INCLUDE(${DYNAMINDDEV}/DynaMindDevCmake.txt)
ELSE()
    MESSAGE(FATAL_ERROR "Could not find dynamind. Maybe not installed ?")
ENDIF()


FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui QtXml QtSql REQUIRED)
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${DYNAMIND_INCLUDE_DIR})


SET(LIBRARY_OUTPUT_PATH ${vibe2urbansim_BINARY_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${vibe2urbansim_BINARY_DIR})

ADD_SUBDIRECTORY(src/urbansimmodules)
ADD_SUBDIRECTORY(scripts)
ADD_SUBDIRECTORY(3rdparty)





