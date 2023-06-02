project (ainb)

add_library(${PROJECT_NAME} 
	src/ainb.cpp
	src/DataBlock.cpp
	src/A_Block.cpp
	src/B_Block.cpp
)

target_link_libraries(${PROJECT_NAME}
	nin-io::util
)

add_library(nin-io::ainb ALIAS ${PROJECT_NAME})



target_include_directories( ${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)