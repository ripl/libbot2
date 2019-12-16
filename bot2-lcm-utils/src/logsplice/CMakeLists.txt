add_executable(bot-lcm-logsplice
    lcm-logsplice.c)
target_link_libraries(bot-lcm-logsplice ${LCM_NAMESPACE}lcm GLib2::glib)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-lcm-logsplice)

