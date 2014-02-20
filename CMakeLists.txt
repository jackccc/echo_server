cmake_minimum_required(VERSION 2.8)
project(echo_server)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(echo_server echo_server.cpp)

find_package(Boost REQUIRED system)

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(echo_server ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS echo_server RUNTIME DESTINATION bin)
