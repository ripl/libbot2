add_executable(bot-lcm-logsplice
    lcm-logsplice.c)
target_link_libraries(bot-lcm-logsplice
  PRIVATE GLib2::glib ${LCM_NAMESPACE}lcm
)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-lcm-logsplice)

