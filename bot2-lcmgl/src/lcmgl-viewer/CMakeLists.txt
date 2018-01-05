find_package(bot2-vis REQUIRED)
find_package(GTK2 REQUIRED)

add_executable(bot-lcmgl-viewer
    main.c
    udp_util.c
    view_menu.c
    )
target_link_libraries(bot-lcmgl-viewer bot2-lcmgl-renderer libbot::bot2-vis libbot::bot2-core ${GTK2_LIBRARIES})
target_include_directories(bot-lcmgl-viewer PUBLIC ${GTK2_INCLUDE_DIRS})
set_property(TARGET bot-lcmgl-viewer PROPERTY C_STANDARD 99)

pods_install_executables(EXPORT ${POD_NAME} bot-lcmgl-viewer)
