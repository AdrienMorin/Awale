project(Server)

add_executable(server server2.c actionManager.c
        csvparser.c)
target_link_libraries(server PRIVATE csv_parser awale)

configure_file(data/users.csv ${CMAKE_CURRENT_BINARY_DIR}/data/users.csv COPYONLY)