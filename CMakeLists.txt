cmake_minimum_required(VERSION 3.1...3.29.5)

#####
## Project
#####
project(lc3 VERSION 1.0.0 LANGUAGES CXX)

######
## Set CXX Standard
######
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#######
## Include Directories
#######

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

#######
# Testing Setup
#######
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    message(STATUS "Enabling tests")
    enable_testing()
    add_subdirectory(tests)
else()
    message(WARNING "Tests directory not found - skipping tests")
endif()


# -----------------
# Add the executable
# -----------------

add_executable(
    ${CMAKE_PROJECT_NAME}
    src/main.cpp
)