cmake_minimum_required (VERSION 3.16)

project(ainb VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

option(COMPILE_CMD "Compile the command line tool(s)" OFF)

add_library(ainb SHARED
	./src/ainb.cpp
	./src/StringList.cpp
	./src/SequenceNode.cpp
	./src/SequenceHandler.cpp
	./src/ParameterHandler.cpp
 )

add_library(ainb_static STATIC
	./src/ainb.cpp
	./src/StringList.cpp
	./src/SequenceNode.cpp
	./src/SequenceHandler.cpp
	./src/ParameterHandler.cpp
 )

target_include_directories(ainb PUBLIC ./src/include)
target_include_directories(ainb_static PUBLIC ./src/include)

add_library(ainb::ainb ALIAS ainb_static)

# Command line tool(s)
if(COMPILE_CMD)
	add_subdirectory(cmd)
endif()
