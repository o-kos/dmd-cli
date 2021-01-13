cmake_minimum_required(VERSION 3.17)
project(dmd-term)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
)
FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(APP_SRC main.cpp loren.hpp datasource.hpp dmd-render.hpp dmd-render.cpp)
add_executable(dmd-term ${APP_SRC})
target_link_libraries(dmd-term
    PRIVATE screen
    PRIVATE dom
#        PRIVATE component # Not needed for this example.
)

# ryml
add_subdirectory(ext/rapidyaml ryml)
target_link_libraries(dmd-term PRIVATE ryml)

enable_testing()
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()