project(app)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
	nin-io::ainb
	nin-io::util
)