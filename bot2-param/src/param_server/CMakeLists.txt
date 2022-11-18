# Create an executable program bot-param-server
add_executable(bot-param-server param_server.c lcm_util.c)
target_link_libraries(bot-param-server
  PRIVATE GLib2::glib ${LCM_NAMESPACE}lcm bot2-param-client lcmtypes_bot2-param
)

# Create an executable program bot-param-tool
add_executable(bot-param-tool param_tool.c)
target_link_libraries(bot-param-tool
  PRIVATE GLib2::glib ${LCM_NAMESPACE}lcm bot2-param-client lcmtypes_bot2-param
)

install(TARGETS bot-param-server bot-param-tool
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
