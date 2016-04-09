set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Qt5 COMPONENTS Widgets)

set(INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include/stream9/qt")
set(SRCS
    abstract_widget_list.cpp
    mixin/action_view_p.cpp
    mixin/menu_view_p.cpp
    mixin/tool_bar_view_p.cpp
    mixin/tool_bar/drag_p.cpp
    ${INCLUDE_PATH}/abstract_widget_list.hpp
    ${INCLUDE_PATH}/mixin/action_view_p.hpp
	${INCLUDE_PATH}/mixin/menu_view_p.hpp
	${INCLUDE_PATH}/mixin/tool_bar_view_p.hpp
)

include_directories(${INCLUDE_PATH})
include_directories("${INCLUDE_PATH}/mixin")

add_library(stream9_qt STATIC ${SRCS})
target_link_libraries(stream9_qt Qt5::Widgets)
install(TARGETS stream9_qt
    ARCHIVE DESTINATION lib
)
