cmake_minimum_required(VERSION 2.6)

project(vibe2urbansim)
if (NOT WIN32)
	set(CMAKE_CXX_FLAGS "-frounding-math" )
endif()
set(CMAKE_MODULE_PATH 
  "${vibe2urbansim_SOURCE_DIR}/CMake"
  "${vibe2urbansim_SOURCE_DIR}/CMake/Utils" 
  "${vibe2urbansim_SOURCE_DIR}/CMake/Packages"
)



find_package(Boost 1.38.0 COMPONENTS python program_options date_time REQUIRED)
find_package(PythonLibs REQUIRED)
find_package(Qt4 COMPONENTS QtCore QtGui QtXml QtSql REQUIRED)
include(${QT_USE_FILE})
include_directories(${Boost_INCLUDE_DIRS} ${QT_QTCORE_INCLUDE_DIR} ${PYTHON_INCLUDE_PATH})

find_package(vibe2core)


set(Boost_USE_STATIC_LIBS OFF)

set(LIBRARY_OUTPUT_PATH ${vibe2urbansim_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${vibe2urbansim_BINARY_DIR})

add_subdirectory(src/urbansimmodules)
add_subdirectory(scripts)
add_subdirectory(3rdparty)





