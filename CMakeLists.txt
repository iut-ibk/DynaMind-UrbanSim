CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(dynamind-urbansim)

FIND_PATH(DYNAMINDDEV DynaMindDevCmake.txt
            PATHS   $ENV{DYNAMIND_DIR}/include
            PATH_SUFFIXES dynamindcore/CMake)

IF(DYNAMIND_INCLUDE_DIR)
        MESSAGE("Compiled from toolbox")
        MESSAGE("DYNAMIND_INCLUDE_DIR ${DYNAMIND_INCLUDE_DIR}")
        SET (DYNAMIND_CORE_LIBRARY dynamindcore)
        SET (DYNAMIND_TOOLBOX_LIBRARY dynamindtoolbox)
        MESSAGE("${DYNAMIND_CORE_LIBRARY}")
ELSE()
        MESSAGE("Compiled from stand alone")
        FIND_PATH(DYNAMINDDEV DynaMindDevCmake.txt
        PATHS $ENV{DYNAMIND_DIR}/include
        PATH_SUFFIXES dynamindcore/CMake)

        IF(DYNAMINDDEV)
                INCLUDE(${DYNAMINDDEV}/DynaMindDevCmake.txt)
        ELSE()
                MESSAGE(FATAL_ERROR "Could not find dynamind. Maybe not installed ?")
        ENDIF()

ENDIF()

FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui REQUIRED)
SET(QT_USE_QTSQL 1)

INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${DYNAMIND_INCLUDE_DIR})

IF(CMAKE_BUILD_TYPE STREQUAL Debug)
        MESSAGE(STATUS "Build Type = Debug")
        ADD_DEFINITIONS(-DDEBUG)
ELSE()
        MESSAGE(STATUS "Build Type = Release")
ENDIF()
IF(DYNAMIND_TOOLBOX_BUILD_DIR)
    SET(LIBRARY_OUTPUT_PATH ${DYNAMIND_TOOLBOX_BUILD_DIR}/Modules)
    SET(EXECUTABLE_OUTPUT_PATH ${DYNAMIND_TOOLBOX_BUILD_DIR}/Modules)
    SET(SCRIPT_PATH ${DYNAMIND_TOOLBOX_BUILD_DIR}/PythonModules )
    SET(3RDPARTY_PATH ${DYNAMIND_TOOLBOX_BUILD_DIR}/3rdParty )
ELSE()
    SET(LIBRARY_OUTPUT_PATH ${dynamind-urbansim_BINARY_DIR})
    SET(EXECUTABLE_OUTPUT_PATH ${dynamind-urbansim_BINARY_DIR})
    SET(SCRIPT_PATH ${dynamind-urbansim_BINARY_DIR}/PythonModules )
    SET(3RDPARTY_PATH ${dynamind-urbansim_BINARY_DIR}/3rdParty )
ENDIF()

ADD_SUBDIRECTORY(src/urbansimmodules)
ADD_SUBDIRECTORY(scripts)
ADD_SUBDIRECTORY(3rdparty)





