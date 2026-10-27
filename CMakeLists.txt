cmake_minimum_required(VERSION 3.16)
project(lab4_binary_search_tree LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

add_library(binary_search_tree src/binary_search_tree.cpp)
target_include_directories(binary_search_tree PUBLIC include)
target_compile_features(binary_search_tree PUBLIC cxx_std_17)

add_executable(lab4 src/main.cpp)
target_link_libraries(lab4 PRIVATE binary_search_tree)

function(enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

enable_warnings(binary_search_tree)
enable_warnings(lab4)


