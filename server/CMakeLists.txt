cmake_minimum_required(VERSION 3.15)

# Set the project name
project(TCPServer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost package
find_package(Boost REQUIRED)

set(EXEC_NAME "server")

# Add the executable
add_executable(${EXEC_NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TCPServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Processors.cpp
)

# Include directories
target_include_directories(${EXEC_NAME} PRIVATE 
                    ${Boost_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link the Boost libraries
target_link_libraries(${EXEC_NAME} PRIVATE ${Boost_LIBRARIES})