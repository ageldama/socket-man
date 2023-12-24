cmake_minimum_required(VERSION 3.28)
project(cpp-asio-coro VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

add_executable(tcp_serv
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp_serv_main.cpp)

target_include_directories(tcp_server 
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(tcp_serv
  PRIVATE
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        src/pokemon_names.cpp
    )
