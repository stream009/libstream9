find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(/home/stream/work/stream9/include)

add_executable(url_test
    ${CMAKE_SOURCE_DIR}/src/url.cpp
    constructor.cpp
    accessor.cpp
    query.cpp
    operator.cpp
    modifier.cpp
)
target_link_libraries(url_test
    -lhttp_parser ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES}
)
