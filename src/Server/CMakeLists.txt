project(Server)

add_executable(server server2.c actionManager.c
        csvparser.c)

target_link_libraries(server PRIVATE cjson csv_parser awale)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/cJSON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/CsvParser)

configure_file(data/users.csv ${CMAKE_CURRENT_BINARY_DIR}/data/users.csv COPYONLY)