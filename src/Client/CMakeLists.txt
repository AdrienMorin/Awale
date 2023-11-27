project(Client)

add_executable(client client2.c)
target_link_libraries(client PRIVATE csv_parser awale)

target_compile_options(client PRIVATE -lm)