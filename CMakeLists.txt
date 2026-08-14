cmake_minimum_required(VERSION 3.20)
project(ai_cloud_sandbox VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(AI_CLOUD_BUILD_TESTS "Build tests" ON)

add_subdirectory(src)

if(AI_CLOUD_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
