cmake_minimum_required(VERSION 3.21)
project(Paddle)

set(CMAKE_CXX_STANDARD 17)

set(SFML_ROOT "/SFML-2.5.1")
set(SFML_INCLUDE_DIR "C:/SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "C:/SFML-2.5.1/lib")

set(SFML_STATIC_LIBRARIES TRUE)

include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio)

add_executable(FinalPong main.cpp pong_defs.h pong_defs.h)
target_link_libraries(FinalPong sfml-graphics sfml-window sfml-system)