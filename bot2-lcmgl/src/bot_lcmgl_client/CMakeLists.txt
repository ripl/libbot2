file(GLOB c_files *.c)
file(GLOB h_files *.h)

set(libname bot2-lcmgl-client)

add_library(${libname} ${c_files})
target_link_libraries(${libname}
  PUBLIC ${LCM_NAMESPACE}lcm
  PRIVATE ZLIB::ZLIB lcmtypes_bot2-lcmgl
)

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(${libname} PROPERTIES SOVERSION 1)

install(TARGETS ${libname}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

pods_install_headers(${h_files} DESTINATION bot_lcmgl_client)

set(REQUIRED_PACKAGES lcm >= 1.4)
pods_install_pkg_config_file(bot2-lcmgl-client
    LIBS -l${libname}
    REQUIRES ${REQUIRED_PACKAGES}
    VERSION 0.0.1)
