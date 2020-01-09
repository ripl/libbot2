add_executable(bot-lcm-logsplice
    lcm-logsplice.c)
target_link_libraries(bot-lcm-logsplice
  PRIVATE GLib2::glib ${LCM_NAMESPACE}lcm
)

install(TARGETS bot-lcm-logsplice
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
