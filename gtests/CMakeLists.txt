project(GoogleTest_run)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(JSON_BuildTests OFF CACHE INTERNAL "")

include_directories(../include
        ../src
        )

add_executable(GoogleTest_run
        InvertedIndex_test.cpp
        SearchServer_test.cpp

        ../src/ConverterJson.cpp ../include/ConverterJson.h
        ../src/InvertedIndex.cpp ../include/InvertedIndex.h
        ../src/SearchServer.cpp ../include/SearchServer.h
        )

target_link_libraries(GoogleTest_run PRIVATE gtest gtest_main PRIVATE nlohmann_json::nlohmann_json)