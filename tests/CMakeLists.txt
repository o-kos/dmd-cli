cmake_minimum_required(VERSION 3.17)
project(dmd-term_tests)

add_subdirectory("${PROJECT_SOURCE_DIR}/../ext/googletest" "ext/googletest")

add_executable(dmd-term_tests phase_test.cpp ../phase.cpp)
target_include_directories(dmd-term_tests PRIVATE ./../)
target_link_libraries(dmd-term_tests
    gtest gtest_main
    screen dom
)




