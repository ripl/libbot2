find_package(GLib2 2.32 MODULE REQUIRED COMPONENTS gthread)
find_package(lcm 1.4 CONFIG REQUIRED)
find_package(Util MODULE REQUIRED)

add_executable(bot-procman-deputy
    lcm_util.c
    procinfo.c
    procman.c
    procman_deputy.c
    signal_pipe.c)
target_link_libraries(bot-procman-deputy
  PRIVATE GLib2::glib ${LCM_NAMESPACE}lcm Util::Util lcmtypes_bot2-procman
)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-procman-deputy)
