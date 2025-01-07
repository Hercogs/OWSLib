cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(OWSLib)

# Set flag if want to use as standlone application
add_definitions(-DOWSLIB_STANDALONE)

# Include the cmake directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(CURL REQUIRED)
find_package(KML REQUIRED)

if(KML_FOUND)
    # message(STATUS "KML found")
else()
    message(FATAL_ERROR "KML not found")
endif()

include_directories(${CURL_INCLUDE_DIRS} ${KML_INCLUDE_DIRS})

set(PWD_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
message(PROJECT_SOURCE_DIR=${PWD_PATH})

include_directories(include)

# Set DEBUG type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Debug compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")


file(GLOB_RECURSE MY_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(app ${PWD_PATH}/src/main.cpp ${MY_SRC})
target_link_libraries(app ${CURL_LIBRARIES} ${KML_LIBRARIES})

