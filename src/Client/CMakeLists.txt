project(Client)

add_executable(client client2.c
        actionManager.c
        actionManager.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/cJSON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/csv_parser)

target_link_libraries(client PRIVATE cjson csv_parser awale)

target_compile_options(client PRIVATE -lm)