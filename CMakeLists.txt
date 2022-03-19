cmake_minimum_required(VERSION 3.8)
project(libbti)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB LIBBTI_SRC
  "src/*.c"
  "include/*.h"
)

add_library(libbti ${LIBBTI_SRC})
target_include_directories(libbti PUBLIC include)