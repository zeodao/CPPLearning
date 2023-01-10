cmake_minimum_required(VERSION 3.15)
macro(fetch_benchmark)
    # fetch content
    include(FetchContent)

    FetchContent_Declare(
     benchmark
     GIT_REPOSITORY git@github.com:google/benchmark.git
     GIT_TAG        v1.7.1
     SOURCE_DIR     ${PROJECT_SOURCE_DIR}/modules/benchmark
    )

    FetchContent_Declare(
     googletest
     GIT_REPOSITORY https://github.com/google/googletest.git
     GIT_TAG        release-1.11.0
     SOURCE_DIR     ${PROJECT_SOURCE_DIR}/modules/benchmark/googletest
    )
    FetchContent_GetProperties(benchmark)
    FetchContent_GetProperties(googletest)
    if (NOT benchmark_POPULATED)
      FetchContent_Populate(benchmark)
      FetchContent_Populate(googletest)
    endif()
    # submodules
    add_subdirectory(${PROJECT_SOURCE_DIR}/modules/benchmark)
endmacro()