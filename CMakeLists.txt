cmake_minimum_required(VERSION 3.17)
project(dmd-term)

set(CMAKE_CXX_STANDARD 17)

set(FTXUI_DIR "./../FTXUI")
#add_library(FTXUI STATIC IMPORTED) # or STATIC instead of SHARED
#set_target_properties(FTXUI PROPERTIES
#    IMPORTED_LOCATION "${FTXUI_DIR}/lib/libscreen.a"
#    INTERFACE_INCLUDE_DIRECTORIES "${FTXUI_DIR}/include"
#    )

#set(APP_SRC "main.cpp")
#add_executable(dmd-term ${APP_SRC})
#target_link_libraries(dmd-term PRIVATE FTXUI) # also adds the required include path

#include_directories(${RX_TERM}/include)

set(APP_SRC main.cpp loren.h)
add_executable(dmd-term ${APP_SRC})
target_include_directories(dmd-term PRIVATE ${FTXUI_DIR}/include)
target_link_directories(dmd-term PRIVATE ${FTXUI_DIR}/lib)
target_link_libraries(dmd-term
        PRIVATE screen
        PRIVATE dom
        PRIVATE component # Not needed for this example.
)
