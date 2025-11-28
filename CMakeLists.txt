cmake_minimum_required(VERSION 3.14)
project(adas_req1)

set(CMAKE_CXX_STANDARD 14)

# --- 自动下载 GoogleTest ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- 设置路径 ---
include_directories(include src)

# --- 生成可执行文件 ---
add_executable(runTests tests/ExecutorTest.cpp src/ExecutorImpl.cpp)

# --- 链接库 ---
target_link_libraries(runTests gtest_main)