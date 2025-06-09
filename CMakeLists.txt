cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
  ascii-rta
  VERSION 0.0.1
  LANGUAGES CXX)

include(FetchContent)

# FMT for string formatting, available project wide
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master)
FetchContent_MakeAvailable(fmt)

add_subdirectory(src)
