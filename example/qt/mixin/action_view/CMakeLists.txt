set(TARGET action_view)

add_executable(${TARGET}
	menu.cpp
	tool_bar.cpp
	main_window.cpp
    main.cpp
)

target_link_libraries(${TARGET} stream9_qt)
