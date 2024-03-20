cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Challenge_18)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
include_directories("/usr/include/cryptopp")
include_directories("/usr/include/crypto++")

add_executable(main main.cpp)

add_library(functions functions.cpp)

target_link_libraries(main functions /usr/lib/x86_64-linux-gnu/libcryptopp.a /usr/lib/x86_64-linux-gnu/libcrypto++.so /usr/lib/x86_64-linux-gnu/libcryptopp.so)
