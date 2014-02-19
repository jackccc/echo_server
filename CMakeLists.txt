project(echo_server)

add_executable(echo_server main.cpp)

install(TARGETS echo_server RUNTIME DESTINATION bin)
