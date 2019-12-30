add_executable(bot-lcm-who
    lcm-who.c
    signal_pipe.c)
target_link_libraries(bot-lcm-who
  PRIVATE GLib2::glib
)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-lcm-who)
