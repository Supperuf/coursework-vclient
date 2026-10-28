cmake_minimum_required(VERSION 3.10)
project(VClient)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(vclient ${SOURCES})

target_link_libraries(vclient PRIVATE OpenSSL::Crypto)
