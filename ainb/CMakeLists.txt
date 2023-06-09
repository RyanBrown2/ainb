project (ainb)

add_library(${PROJECT_NAME} 
	src/ainb.cpp
	"src/BaseCommand.cpp"
	"src/A_Command.cpp"
	"src/B_Command.cpp"
	
	src/StringList.cpp
	src/Parameters.cpp
	src/ParameterNode.cpp
)

target_link_libraries(${PROJECT_NAME}
	nin-io::util
)

add_library(nin-io::ainb ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)