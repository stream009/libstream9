find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(GTest)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(url_test
    constructor.cpp
    accessor.cpp
    query.cpp
    operator.cpp
    modifier.cpp
)
target_link_libraries(url_test
    -lstream9 -lhttp_parser
    ${Boost_LIBRARIES} ${GTEST_BOTH_LIBRARIES}
)
