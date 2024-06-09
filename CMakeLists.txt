cmake_minimum_required(VERSION 3.0)
project(custom-shell)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

execute_process(
    COMMAND sh -c "git describe --tags --always | cut -c 2-"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    RESULT_VARIABLE ERROR_CODE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

include_directories(include)

add_library(${PROJECT_NAME} SHARED ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME} readline)
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${GIT_VERSION})

if(${BUILD_EXAMPLE})
    include_directories(${PROJECT_SOURCE_DIR})
    add_subdirectory(example)
endif()
