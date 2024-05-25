cmake_minimum_required(VERSION 3.0)
project(custom-shell VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} readline)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})

add_executable(${PROJECT_NAME}-example ${PROJECT_NAME}-example.cpp)
target_link_libraries(${PROJECT_NAME}-example
    readline
    ${PROJECT_NAME}
    )
