# Create an executable program param-tester
add_executable(param-tester param_tester.c)
target_link_libraries(param-tester
  PRIVATE ${LCM_NAMESPACE}lcm bot2-param-client
)

# Create an executable program param-tester
add_executable(bot-param-dump param_dump.c)
target_link_libraries(bot-param-dump
  PRIVATE ${LCM_NAMESPACE}lcm bot2-param-client lcmtypes_bot2-param
)

# make executable public
pods_install_executables(EXPORT ${PROJECT_NAME} bot-param-dump)
