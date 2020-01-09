add_executable(bot-lcm-who
    lcm-who.c
    signal_pipe.c)
target_link_libraries(bot-lcm-who
  PRIVATE GLib2::glib
)

install(TARGETS bot-lcm-who
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
