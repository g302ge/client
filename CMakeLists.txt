cmake_minimum_required(VERSION 3.15)
project(client VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(asio REQUIRED)
find_package(Threads)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)



