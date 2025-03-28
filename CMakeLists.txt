cmake_minimum_required(VERSION 3.5.0)
project(custom-shell)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

execute_process(
    COMMAND sh -c "git describe --tags --always | cut -c 2-"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    RESULT_VARIABLE ERROR_CODE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME}_obj OBJECT ${PROJECT_NAME}.cpp)
set_property(TARGET ${PROJECT_NAME}_obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_obj>)
target_link_libraries(${PROJECT_NAME}_static readline)
set_target_properties(${PROJECT_NAME}_static PROPERTIES VERSION ${GIT_VERSION})

add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_obj>)
target_link_libraries(${PROJECT_NAME}_shared readline)
set_target_properties(${PROJECT_NAME}_shared PROPERTIES VERSION ${GIT_VERSION})

set_target_properties(${PROJECT_NAME}_static PROPERTIES PUBLIC_HEADER "${PROJECT_NAME}.h")
set_target_properties(${PROJECT_NAME}_shared PROPERTIES PUBLIC_HEADER "${PROJECT_NAME}.h")

install(TARGETS
    ${PROJECT_NAME}_static
    ${PROJECT_NAME}_shared
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION lib/${PROJECT_NAME}
    LIBRARY DESTINATION lib/${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)

if(${BUILD_EXAMPLE})
    include_directories(${PROJECT_SOURCE_DIR})
    add_subdirectory(example)
endif()
