cmake_minimum_required(VERSION 3.15)
project(client VERSION "0.0.1")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(asio REQUIRED)
find_package(Threads)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED) 
find_package(llhttp REQUIRED)


file(
    GLOB
    CLIENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/client/*/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/client/*/*.cpp
)
message(STATUS "SOURCE ${CLIENT_SOURCES}")
add_library(
    client
    ${CLIENT_SOURCES}
)

target_include_directories(
    client
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${llhttp_INCLUDE_DIR}
    PRIVATE ${asio_INCLUDE_DIR}
    PRIVATE ${OpenSSL_INCLUDE_DIR}
    PRIVATE ${ZLIB_INCLUDE_DIR}
)
target_link_libraries(
    client
    Threads::Threads
    asio::asio
    ${OpenSSL_COMPONENTS}
    ZLIB::ZLIB
    llhttp::llhttp
)

function(client_add_test TEST_NAME SOURCES)
    find_package(GTest REQUIRED)
    add_executable(${TEST_NAME} "")
    target_sources(${TEST_NAME} PRIVATE ${SOURCES})
    target_include_directories(${TEST_NAME} 
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include 
        PRIVATE ${GTEST_INCLUDE_DIRS}
        PRIVATE ${llhttp_INCLUDE_DIR}
        PRIVATE ${asio_INCLUDE_DIR}
        PRIVATE ${OpenSSL_INCLUDE_DIR}
        PRIVATE ${ZLIB_INCLUDE_DIR}
    )
    target_link_libraries(${TEST_NAME} client GTest::gtest GTest::gtest_main GTest::gmock)
    add_dependencies(${TEST_NAME} client)
endfunction()

client_add_test(streambuf_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/streambuf_test.cc)
client_add_test(io_pool_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/io_pool_test.cc)


