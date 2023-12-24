cmake_minimum_required(VERSION 3.27)
project(cpp-asio-coro VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

find_package(PkgConfig REQUIRED)

pkg_check_modules(spdlog spdlog REQUIRED)

find_package(Boost
  1.80
  REQUIRED
  COMPONENTS coroutine context system program_options)

add_executable(tcp_serv)

target_include_directories(tcp_serv
  PRIVATE
  ${spdlog_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_sources(tcp_serv
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp_serv_listener.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp_serv_main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp_serv_progopts.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/pokemon_names.cpp)

target_link_libraries(tcp_serv ${spdlog_LIBRARIES} ${Boost_LIBRARIES})
