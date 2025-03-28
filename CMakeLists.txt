cmake_minimum_required(VERSION 3.10)
project(BasicOptionsPricing VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler warnings
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Header-only library
add_library(options_pricing INTERFACE)
target_include_directories(options_pricing INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Main example executable
add_executable(options_pricing_example main.cpp)
target_link_libraries(options_pricing_example PRIVATE options_pricing)

# Tests (optional, only if Catch2 is available)
option(BUILD_TESTS "Build the tests" ON)
if(BUILD_TESTS)
    find_package(Catch2 QUIET)
    if(Catch2_FOUND)
        add_executable(options_pricing_tests tests.cpp)
        target_link_libraries(options_pricing_tests PRIVATE options_pricing Catch2::Catch2)
    else()
        message(STATUS "Catch2 not found. Tests will not be built.")
    endif()
endif()

# Documentation (optional, only if Doxygen is available)
option(BUILD_DOCS "Build documentation" OFF)
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message(STATUS "Doxygen configured")

        add_custom_target(docs
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    else()
        message(STATUS "Doxygen not found. Documentation will not be built.")
    endif()
endif()

# Installation
install(TARGETS options_pricing
    EXPORT options_pricing-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT options_pricing-targets
    FILE options_pricing-targets.cmake
    NAMESPACE options_pricing::
    DESTINATION lib/cmake/options_pricing
)

# Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/options_pricing-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Configure the config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/options_pricing-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/options_pricing-config.cmake"
    INSTALL_DESTINATION lib/cmake/options_pricing
)

# Install the config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/options_pricing-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/options_pricing-config-version.cmake"
    DESTINATION lib/cmake/options_pricing
)