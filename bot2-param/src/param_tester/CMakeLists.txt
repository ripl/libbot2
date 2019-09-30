# Create an executable program param-tester
add_executable(param-tester param_tester.c)
target_link_libraries(param-tester bot2-param-client ${LCM_NAMESPACE}lcm GLib2::glib lcmtypes_bot2-param)
set_property(TARGET param-tester PROPERTY C_STANDARD 99)

# Create an executable program param-tester
add_executable(bot-param-dump param_dump.c)
target_link_libraries(bot-param-dump bot2-param-client ${LCM_NAMESPACE}lcm GLib2::glib lcmtypes_bot2-param)
set_property(TARGET bot-param-dump PROPERTY C_STANDARD 99)

# make executable public
pods_install_executables(EXPORT ${PROJECT_NAME} bot-param-dump)
