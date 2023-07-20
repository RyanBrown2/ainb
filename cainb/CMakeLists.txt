# This is a wrapper for the ainb library to make c# support easier

project(cainb VERSION 0.1.0)

add_library(cainb SHARED
	./src/cainb.cpp
 "src/include/data_types.h")

target_link_libraries(cainb
	ainb::ainb
)

target_include_directories(cainb PUBLIC ./src/include)

set(CPACK_PROJECT_NAME cainb)
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
