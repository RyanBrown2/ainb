project (ainb)

add_library(${PROJECT_NAME} 
	src/ainb.cpp
	src/StringList.cpp
	src/ParameterHandler.cpp
)

target_link_libraries(${PROJECT_NAME}
	nin-io::util
)

add_library(nin-io::ainb ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)