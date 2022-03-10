add_executable(bot-lcm-logfilter
    lcm-logfilter.c)
target_link_libraries(bot-lcm-logfilter
  PRIVATE GLib2::glib ${LCM_NAMESPACE}lcm
)

install(TARGETS bot-lcm-logfilter
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
