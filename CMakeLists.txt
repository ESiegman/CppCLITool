cmake_minimum_required(VERSION 3.25)
project(CppCLITool 
    VERSION 0.1.0 
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
find_package(yaml-cpp QUIET)

if(NOT yaml-cpp_FOUND)
    FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG 0.8.0
    )
    FetchContent_MakeAvailable(yaml-cpp)
endif()

file(GLOB_RECURSE SRC_FILES 
    "src/*.cpp"
)

add_executable(cpp-cli ${SRC_FILES})

target_compile_features(cpp-cli PRIVATE cxx_std_23)

set_target_properties(cpp-cli PROPERTIES 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(cpp-cli PRIVATE include)

target_link_libraries(cpp-cli PRIVATE yaml-cpp::yaml-cpp)

if(MSVC)
    target_compile_options(cpp-cli PRIVATE /W4 /WX /permissive-)
else()
    target_compile_options(cpp-cli PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
