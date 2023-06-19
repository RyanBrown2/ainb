project (nin-io-wrapper)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/wrapper )

add_library(${PROJECT_NAME} SHARED
	"ainb/ainb_wrapper.cpp"
	 "ainb/SequenceNode.cpp" "ainb/SequenceNode.h")

target_link_libraries(${PROJECT_NAME}
	nin-io::ainb
	nin-io::util
)

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)



