add_executable(bot-lcm-logfilter
    lcm-logfilter.c)
target_link_libraries(bot-lcm-logfilter
  PRIVATE GLib2::glib ${LCM_NAMESPACE}lcm
)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-lcm-logfilter)
