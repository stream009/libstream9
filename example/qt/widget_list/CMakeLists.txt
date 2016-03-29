set(CMAKE_AUTOMOC On)
set(CMAKE_AUTOUIC On)
set(CMAKE_INCLUDE_CURRENT_DIR On)
find_package(Qt5 COMPONENTS Widgets)

add_executable(widget_list
    main.cpp
    main_window.cpp
    item_widget.cpp
    item_actions.cpp
    sample_model.cpp
)

target_link_libraries(widget_list stream9_qt)
