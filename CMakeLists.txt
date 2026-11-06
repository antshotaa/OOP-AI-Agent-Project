cmake_minimum_required(VERSION 3.20)
project(OllamaTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
    add_compile_options(/utf-8 /EHsc)  
endif()

find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp)
add_executable(OllamaTest ${SRC})

target_include_directories(OllamaTest PRIVATE src)

target_link_libraries(OllamaTest PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)