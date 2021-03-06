cmake_minimum_required(VERSION 3.17)
project(dmd-term)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Winvalid-pch)

set(APP_SRC main.cpp datasource.hpp dmd-render.hpp dmd-state.hpp loren.hpp phase.hpp phase.cpp pch.h)
add_executable(dmd-term ${APP_SRC})
target_precompile_headers(dmd-term PRIVATE pch.h)

# FTXUI
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "Set to ON to build examples" FORCE)
set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "Generate the install target" FORCE)
set(FTXUI_BUILD_DOCS     OFF CACHE BOOL "Set to ON to build docs"     FORCE)
add_subdirectory("ext/FTXUI")
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