cmake_minimum_required(VERSION 3.30)
project(camTranslator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenSSL REQUIRED)
find_package(OpenCV REQUIRED)

if (APPLE)
    find_package(raylib REQUIRED)
endif ()
#find_package(OpenGL REQUIRED)

if (WIN32)
    include_directories("E:/raylib-5.5_win64_msvc16/include")
    link_directories("E:/raylib-5.5_win64_msvc16/lib")
endif ()

add_executable(camTranslator
        main.cpp
        httplib.h
        Configuration.cpp
        Configuration.h
        Translator.cpp
        Translator.h
        ImageAdjuster.cpp
        ImageAdjuster.h)

#gdi32 winmm OpenGL::GL ${OPENGL_INCLUDE_DIRS}

target_link_libraries(camTranslator PRIVATE ${OpenCV_LIBS} OpenSSL::SSL OpenSSL::Crypto raylib)

if (WIN32)
    target_link_libraries(camTranslator PRIVATE ws2_32 gdi32 winmm)
endif()

include_directories(${OPENSSL_INCLUDE_DIR} ${OpenCV_INCLUDE_DIRS})

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()

message(STATUS "OpenCV Version: ${OpenCV_VERSION}")
message(STATUS "OpenCV Include Directories: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenSSL Include Directories: ${OPENSSL_INCLUDE_DIR}")
