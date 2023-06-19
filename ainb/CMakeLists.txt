project (ainb)

add_library(${PROJECT_NAME}
	src/ainb.cpp
	src/StringList.cpp
	src/ParameterHandler.cpp
	src/EntryPointCommand.cpp
	src/ExecutionCommand.cpp
	src/CommandBody.cpp
	src/SequenceHandler.cpp)

target_link_libraries(${PROJECT_NAME}
	nin-io::util
)

add_library(nin-io::ainb ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
	PUBLIC ${PROJECT_SOURCE_DIR}/include
)