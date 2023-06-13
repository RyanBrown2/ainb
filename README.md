# nin-io
Parsing file formats used in Nintendo games. Focusing on ainb files, might add more formats in the future.

## Building For Command Line Usage
First clone the repository and then run:

```
mkdir build
cd build
cmake ..
make
```
The build output will be in `/build/bin/`.
```
Usage: ./ainb_parse <file>
```

Note: This tool will output data from the .ainb file into a .yml file. This is mainly just for file insight. At some point, I will most likely create a more graphical application that supports file editing.

## Using Library
For cmake projects, add the following to your `CMakeLists.txt`:
```
include(FetchContent)
FetchContent_Declare(nin-io
	 GIT_REPOSITORY https://github.com/RyanBrown2/nin-io.git
	 GIT_TAG master)
FetchContent_GetProperties(nin-io)
if(NOT nin-io_POPULATED)
  message(STATUS "Fetching nin-io...")
  FetchContent_Populate(nin-io)
  add_subdirectory(${nin-io_SOURCE_DIR} ${nin-io_BINARY_DIR})
endif()

target_link_libraries(${PROJECT_NAME}
	nin-io::ainb
	nin-io::util
)
```
There's still more functionality I need to add to the library. Once this is done I will work on library documentation.
