find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories("${CMAKE_SOURCE_DIR}/include")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 11)
add_executable(c++11_test
    apply.cpp
    invoke.cpp
    index_sequence.cpp
)
target_link_libraries(c++11_test
    ${GTEST_BOTH_LIBRARIES}
)

set(CMAKE_CXX_STANDARD 14)
add_executable(c++14_test
    apply.cpp
    invoke.cpp
    index_sequence.cpp
)
target_link_libraries(c++14_test
    ${GTEST_BOTH_LIBRARIES}
)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(url)
