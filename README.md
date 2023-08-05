# AINB
Library for AINB files.

## Building For Command Line Usage
First clone the repository and then run:
```
mkdir build
cd build
cmake -D COMPILE_CMD=ON ..
make
```
The build output will be in `/build/bin/`.
```
Usage: ./ainb_to_yaml <file>
```

Note: This tool will output data from the .ainb file into a .yml file. This is mainly just for file insight. At some point, I am working on a graphical application that supports file editing.

## CMake Usage
Add the following to your CMakeLists.txt file:
```
include(FetchContent)
FetchContent_Declare(ainb
	 GIT_REPOSITORY https://github.com/RyanBrown2/ainb.git
	 GIT_TAG master)
FetchContent_GetProperties(ainb)
if(NOT ainb_POPULATED)
  message(STATUS "Fetching ainb...")
  FetchContent_Populate(ainb)
  add_subdirectory(${ainb_SOURCE_DIR} ${ainb_BINARY_DIR})
endif()
```

There's still more functionality I need to add to the library. Once this is done I will work on library documentation.
