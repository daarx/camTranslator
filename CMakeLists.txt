cmake_minimum_required(VERSION 3.30)
project(camTranslator)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(camTranslator main.cpp httplib.h
        Configuration.cpp
        Configuration.h)

target_link_libraries(camTranslator PRIVATE ws2_32 ${OpenCV_LIBS} OpenSSL::SSL OpenSSL::Crypto)

include_directories(${OPENSSL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

message(STATUS "OpenCV Version: ${OpenCV_VERSION}")
message(STATUS "OpenCV Include Directories: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenSSL Include Directories: ${OPENSSL_INCLUDE_DIR}")
