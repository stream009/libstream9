set(CMAKE_AUTOMOC On)
set(CMAKE_AUTOUIC On)
set(CMAKE_INCLUDE_CURRENT_DIR On)
find_package(Qt5 COMPONENTS Widgets)

add_subdirectory(action_view)
add_subdirectory(menu)
add_subdirectory(tool_bar)
