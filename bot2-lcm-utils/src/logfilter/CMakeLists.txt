add_executable(bot-lcm-logfilter
    lcm-logfilter.c)
target_link_libraries(bot-lcm-logfilter ${LCM_NAMESPACE}lcm GLib2::glib)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-lcm-logfilter)
