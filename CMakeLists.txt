cmake_minimum_required(VERSION 3.15)

set(HUNTER_PACKAGES Catch)
include(FetchContent)
FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)
FetchContent_MakeAvailable(SetupHunter)

project(AOC VERSION 0.0.1)

find_package(Catch2 CONFIG REQUIRED)

add_subdirectory(common)

add_subdirectory(2019)
