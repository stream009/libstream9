
add_library(stream9 STATIC
    url.cpp
)
install(TARGETS stream9
    ARCHIVE DESTINATION lib
)

add_subdirectory(qt)
