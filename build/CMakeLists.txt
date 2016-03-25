project(game)
cmake_minimum_required(VERSION 2.8)

set(BIN game)
set(SRC_PATH ../src)
set(INSTALL_PATH ./ )
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

aux_source_directory(${SRC_PATH} SOURCES)
include_directories(${SRC_PATH})

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${BIN} ${SOURCES})
target_link_libraries(${BIN} ${CURSES_LIBRARIES})