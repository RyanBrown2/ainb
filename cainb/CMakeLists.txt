# This is a wrapper for the ainb library to make c# support easier

project(cainb)

add_library(cainb SHARED
	./src/cainb.cpp
)

target_link_libraries(cainb
	ainb::ainb
)

target_include_directories(cainb PUBLIC ./src/include)
