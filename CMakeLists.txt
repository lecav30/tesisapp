cmake_minimum_required(VERSION 3.5.0)
project(TesisApp VERSION 0.1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(CMAKE_CXX_STANDARD 17)
set(VCPKG_MANIFEST_MODE ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
# include_directories(${CMAKE_SOURCE_DIR}/include "${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/include")
# link_directories("${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/lib")

# Necessary for MACOS
find_library(COREFOUNDATION_LIBRARY CoreFoundation)
# Necessary for Linux
# find_library(RESOLV_LIBRARY resolv)

find_package(fmt CONFIG REQUIRED)
find_package(inja CONFIG REQUIRED)
find_package(unofficial-mysql-connector-cpp CONFIG REQUIRED)
find_package(boost_algorithm CONFIG REQUIRED)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(TesisApp ${SOURCES})

# Set target properties
set_target_properties(TesisApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Necessary for MACOS
target_link_libraries(TesisApp PRIVATE ${COREFOUNDATION_LIBRARY} ${RESOLV_LIBRARY})
# Necessary for Linux
# target_link_libraries(TesisApp PRIVATE ${RESOLV_LIBRARY})
target_link_libraries(TesisApp PRIVATE fmt::fmt pantor::inja unofficial::mysql-connector-cpp::connector Boost::algorithm)
