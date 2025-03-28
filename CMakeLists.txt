cmake_minimum_required(VERSION 3.10)
project(BasicOptionsPricing VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler warnings
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Header-only library
add_library(options_pricing INTERFACE)
target_include_directories(options_pricing INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


add_executable(options_pricing_example examples/main.cpp)
target_link_libraries(options_pricing_example PRIVATE options_pricing)