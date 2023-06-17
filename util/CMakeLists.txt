project (util)

add_library(${PROJECT_NAME} 
	src/util.cpp
)

add_library(nin-io::util ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)