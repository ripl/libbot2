file(GLOB c_files *.c)
file(GLOB h_files *.h)

set(libname bot2-lcmgl-client)

add_library(${libname} ${c_files})
target_link_libraries(${libname} PUBLIC ${LCM_NAMESPACE}lcm PRIVATE ${ZLIB_LIBRARIES} lcmtypes_bot2-lcmgl)
target_include_directories(${libname} PRIVATE ${ZLIB_INCLUDE_DIRS})

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(${libname} PROPERTIES SOVERSION 1)

# make both the shared library public
pods_install_libraries(EXPORT ${PROJECT_NAME} ${libname})

pods_install_headers(${h_files} DESTINATION bot_lcmgl_client)

set(REQUIRED_PACKAGES glib-2.0 lcm lcmtypes_bot2-lcmgl)
pods_install_pkg_config_file(bot2-lcmgl-client
    LIBS -l${libname}
    REQUIRES ${REQUIRED_PACKAGES}
    VERSION 0.0.1)
