cmake_minimum_required (VERSION 3.10)

project(ainb VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(ainb SHARED
	./src/ainb.cpp
	./src/StringList.cpp
	./src/SequenceNode.cpp
	./src/SequenceHandler.cpp
	./src/ParameterHandler.cpp
)

target_include_directories(ainb PUBLIC ./src/include)

add_library(ainb::ainb ALIAS ${PROJECT_NAME})

add_subdirectory(cmd)
IF (WIN32)
    # Copy <filename> to build directory
    set(copy_source_dir "${CMAKE_BINARY_DIR}")
    set(copy_dest_dir "${CMAKE_BINARY_DIR}/bin")
    set(copy_file_name "ainb.dll")
    add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${copy_dest_dir}
    )
    add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy "${copy_source_dir}/${copy_file_name}" "${copy_dest_dir}/${copy_file_name}"
	COMMENT "Copying ${copy_file_name} to build directory"
    )
ENDIF()
