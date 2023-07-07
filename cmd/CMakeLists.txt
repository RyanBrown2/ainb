
project(commands)

include(FetchContent)
FetchContent_Declare(yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0e6e28d1a38224fc8172fae0109ea7f673c096db)  # The latest yaml-cpp commit at the time of writing
FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

add_executable("ainb_to_yaml" "ainb_to_yaml.cpp")

target_link_libraries("ainb_to_yaml"
	ainb::ainb
	yaml-cpp::yaml-cpp
)

