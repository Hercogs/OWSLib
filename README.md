# OWS library

### Description
This is C++ package which allows to make web feature service (WFS) requests.
## Setup
### 1. Clone and build
  
```
git clone git@github.com:Hercogs/OWSLib.git
cd OWSLib
mkdir build; cd build
cmake ..; make
```

### 2. Dependecies

This code depends on:
    - libkml
    - libcurl

Example, how to build anything using this library: `g++ main.cpp  -L build/lib -lOWSLib `pkg-config --cflags --libs libkml` -lcurl`

## Usage
To execute compiled object, run: `./app`

In `CMakeLists.txt` file comment out line `add_definitions(-DOWSLIB_STANDALONE)` if you want to use this in another project. 
What it does, it excludes `./src/main.cpp` file from compilation.

