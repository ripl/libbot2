add_library(bot2-param-client
    param_internal.c param_util.c)

find_package(bot2-core CONFIG REQUIRED)
target_include_directories(bot2-param-client
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(bot2-param-client
  PUBLIC ${LCM_NAMESPACE}lcm libbot2::bot2-core
  PRIVATE GLib2::glib lcmtypes_bot2-param
)

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(bot2-param-client PROPERTIES SOVERSION 1)

# make the header public
pods_install_headers(param_client.h param_util.h DESTINATION bot_param)

install(TARGETS bot2-param-client
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# create a pkg-config file for the library, to make it for other software to
# use it.
set(REQUIRED_LIBS lcm >= 1.4 bot2-core)
pods_install_pkg_config_file(bot2-param-client
    LIBS -lbot2-param-client
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
