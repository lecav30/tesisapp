cmake_minimum_required(VERSION 3.5.0)
project(TesisApp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(TesisApp ${SOURCES})

set_target_properties(TesisApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

include(FetchContent)
FetchContent_Declare(
    inja
    GIT_REPOSITORY https://github.com/pantor/inja.git
    GIT_TAG main
)

FetchContent_MakeAvailable(inja)

target_link_libraries(TesisApp PRIVATE inja)