cmake_minimum_required(VERSION 3.17)
project(dmd-term)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("ext/FTXUI")

set(APP_SRC main.cpp datasource.hpp dmd-render.hpp dmd-state.hpp loren.hpp phase.hpp phase.cpp)
add_executable(dmd-term ${APP_SRC})
target_link_libraries(dmd-term
    PRIVATE screen
    PRIVATE dom
)

# ryml
add_subdirectory(ext/rapidyaml ryml)
target_link_libraries(dmd-term PRIVATE ryml)

if (NOT BUILD_MAIN_ONLY)
    enable_testing()
    add_subdirectory(tests)
endif()