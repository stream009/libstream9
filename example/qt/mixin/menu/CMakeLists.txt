set(TARGET menu)

add_executable(${TARGET}
    main_window.cpp
    main.cpp
)

target_link_libraries(${TARGET} stream9_qt)
