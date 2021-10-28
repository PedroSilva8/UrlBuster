cmake_minimum_required(VERSION 3.0.0)
project(UrlBuster VERSION 0.1.0)

cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3")
set(CMAKE_CXX_FLAGS_RELEASE "-g3")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CURL_LIBRARY "-lcurl") 

find_package(CURL REQUIRED) 
find_package(Threads REQUIRED)

add_executable(UrlBuster src/Main.cpp)

add_subdirectory(src/)

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(UrlBuster ${CURL_LIBRARIES})
target_link_libraries(UrlBuster Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)