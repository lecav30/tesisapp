cmake_minimum_required(VERSION 3.5.0)
project(TesisApp VERSION 0.1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(VCPKG_MANIFEST_MODE ON)

find_package(fmt CONFIG REQUIRED)
find_package(inja CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(TesisApp ${SOURCES})

# Set target properties
set_target_properties(TesisApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

target_link_libraries(TesisApp PRIVATE fmt::fmt pantor::inja unofficial::mysql-connector-cpp::connector)