cmake_minimum_required(VERSION 3.23)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(gtests)

include_directories(${PROJECT_SOURCE_DIR}/include)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(SearchEngine main.cpp ${sources} ${headers})
target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)
