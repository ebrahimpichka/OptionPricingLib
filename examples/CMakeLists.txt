# Examples CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Create example executable
add_executable(options_pricing_examples main.cpp)

# Add include directory
target_include_directories(options_pricing_examples PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link against the library
target_link_libraries(options_pricing_examples PRIVATE options_pricing)