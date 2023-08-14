cmake_minimum_required (VERSION 3.16)

project(ainb VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(COMPILE_CMD "Compile the command line tool(s)" OFF)
option(COMPILE_TEST "Compile the test tool" OFF)

set(LIB_SOURCE_FILES
	src/ainb.cpp
	src/StringList.cpp
	src/SequenceNode.cpp
	src/SequenceHandler.cpp
	src/ParameterHandler.cpp
	src/AddressManager.cpp
	src/ExternalHandler.cpp 
	src/parameters/ParameterBase.cpp
	src/parameters/InternalParameter.cpp
	src/parameters/CommandParameter.cpp
	src/parameters/ExternalParameter.cpp
)

add_library(ainb SHARED ${LIB_SOURCE_FILES})

add_library(ainb_static STATIC ${LIB_SOURCE_FILES})

target_include_directories(ainb PUBLIC ./include)
target_include_directories(ainb_static PUBLIC ./include)

add_library(ainb::ainb ALIAS ainb_static)

# Command line tool(s)
if(COMPILE_CMD)
	add_subdirectory(cmd)
endif()

if(COMPILE_TEST)
	add_subdirectory(test)
endif()
