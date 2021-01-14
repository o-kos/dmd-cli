cmake_minimum_required(VERSION 3.17)
project(dmd-term)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("ext/FTXUI")

set(APP_SRC main.cpp loren.hpp dmd-render.hpp dmd-state.hpp phase.cpp)
add_executable(dmd-term ${APP_SRC})
target_link_libraries(dmd-term
    PRIVATE screen
    PRIVATE dom
)

enable_testing()
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()