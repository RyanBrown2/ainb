# AINB
Library for AINB files.

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

There's still more functionality I need to add to the library. Once this is done I will work on library documentation.
