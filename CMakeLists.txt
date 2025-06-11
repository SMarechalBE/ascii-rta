cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  ascii-rta
  LANGUAGES CXX)

include(FetchContent)

# FMT for string formatting, available project wide
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master)
FetchContent_MakeAvailable(fmt)

# Span library for C++11 and more
FetchContent_Declare(
        span-lite
        GIT_REPOSITORY https://github.com/martinmoene/span-lite.git
        GIT_TAG master)
FetchContent_MakeAvailable(span-lite)

add_subdirectory(src)
