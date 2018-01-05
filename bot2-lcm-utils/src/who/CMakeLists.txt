add_executable(bot-lcm-who
    lcm-who.c
    signal_pipe.c)
set_property(TARGET bot-lcm-who PROPERTY C_STANDARD 99)
target_link_libraries(bot-lcm-who ${LCM_NAMESPACE}lcm GLib2::glib)

pods_install_executables(EXPORT ${POD_NAME} bot-lcm-who)
