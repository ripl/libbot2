# Create an executable program bot-param-server
add_executable(bot-param-server param_server.c lcm_util.c)
target_link_libraries(bot-param-server bot2-param-client ${LCM_NAMESPACE}lcm GLib2::glib lcmtypes_bot2-param)
set_property(TARGET bot-param-server PROPERTY C_STANDARD 99)

# Create an executable program bot-param-tool
add_executable(bot-param-tool param_tool.c)
target_link_libraries(bot-param-tool bot2-param-client ${LCM_NAMESPACE}lcm GLib2::glib lcmtypes_bot2-param)
set_property(TARGET bot-param-tool PROPERTY C_STANDARD 99)

# make executables public
pods_install_executables(EXPORT ${POD_NAME} bot-param-server bot-param-tool)

