cmake_minimum_required(VERSION 3.0)
project(custom-shell-example)

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.cpp)
target_link_libraries(${PROJECT_NAME}
    readline
    custom-shell_shared
    )
