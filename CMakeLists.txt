cmake_minimum_required(VERSION 3.16)
project(SQLiteToCsv VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 14)

add_compile_options(-lsqlite3)
add_compile_options(-lstdc)
add_compile_options(-lpthread)
add_compile_options(-ldl)

add_executable(SQLiteToCsv main.c main.h)

target_link_libraries(SQLiteToCsv LINK_PUBLIC sqlite3)
target_link_libraries(SQLiteToCsv LINK_PUBLIC pthread)
target_link_libraries(SQLiteToCsv LINK_PUBLIC dl)