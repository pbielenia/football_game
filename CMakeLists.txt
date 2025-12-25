cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(football_game)

set(EXECUTABLE_NAME ${PROJECT_NAME})
set(CMAKE_COLOR_DIAGNOSTICS ON CACHE BOOL "" FORCE)

find_package(raylib 5.5 REQUIRED)

add_executable(${EXECUTABLE_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/agent.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/pitch_controller.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/renderer.cpp"
)
target_include_directories(${EXECUTABLE_NAME}
    PUBLIC
        include
)
target_compile_features(${EXECUTABLE_NAME}
    PRIVATE
        cxx_std_23
)
target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE
        raylib
)
