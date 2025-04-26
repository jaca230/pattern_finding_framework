cmake_minimum_required(VERSION 3.10)
project(PatternFinder)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Automatically include all subdirectories under include/
file(GLOB_RECURSE ALL_INCLUDE_PATHS LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/include/*)
foreach(path ${ALL_INCLUDE_PATHS})
    if(IS_DIRECTORY ${path})
        include_directories(${path})
    endif()
endforeach()

# Collect and print all header files
file(GLOB_RECURSE HEADER_FILES "include/**/*.h")
message(STATUS "Header files found:")
foreach(header ${HEADER_FILES})
    message(STATUS "  ${header}")
endforeach()

# Collect source files
file(GLOB_RECURSE SRC_FILES "src/**/*.cpp")

# Add main.cpp as its own executable
add_executable(pattern_finder main.cpp ${SRC_FILES})
