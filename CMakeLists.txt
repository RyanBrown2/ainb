cmake_minimum_required (VERSION 3.10)

project(ainb VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ainb SHARED
	./src/ainb.cpp
	./src/include/ainb/ainb.h
  "src/include/ainb/util.h" "src/include/ainb/StringList.h" "src/StringList.cpp" "src/include/ainb/SequenceNode.h" "src/SequenceNode.cpp" "src/include/ainb/SequenceHandler.h" "src/SequenceHandler.cpp")

target_include_directories(ainb PUBLIC ./src/include)
target_include_directories(ainb PRIVATE ./src)

