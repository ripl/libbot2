find_package(GLib2 2.32 MODULE REQUIRED)
find_package(lcm 1.4 CONFIG REQUIRED)
find_package(M MODULE REQUIRED)

file(GLOB c_files *.c)
file(GLOB h_files *.h)

add_library(${PROJECT_NAME} ${c_files})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    GLib2::glib
    ${LCM_NAMESPACE}lcm
    M::M
    lcmtypes_bot2-core
)

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

pods_install_libraries(EXPORT ${PROJECT_NAME} ${PROJECT_NAME})

pods_install_headers(${h_files} DESTINATION bot_core)

set(REQUIRED_LIBS glib-2.0 >= 2.32 lcm >= 1.4 lcmtypes_${PROJECT_NAME})
pods_install_pkg_config_file(${PROJECT_NAME}
    LIBS -lbot2-core -lm
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
