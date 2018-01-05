list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")
find_package(GLib2 REQUIRED)

file(GLOB c_files *.c)
file(GLOB h_files *.h)

add_library(${POD_NAME} ${c_files})

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(${POD_NAME} PROPERTIES SOVERSION 1)

target_link_libraries(${POD_NAME} PUBLIC ${LCM_NAMESPACE}lcm-coretypes ${LCM_NAMESPACE}lcm GLib2::glib PRIVATE m)
target_include_directories(${POD_NAME} PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include> )
set_property(TARGET ${POD_NAME} PROPERTY C_STANDARD 99)

pods_install_libraries(EXPORT ${POD_NAME} ${POD_NAME})

pods_install_headers(${h_files} DESTINATION bot_core)

set(REQUIRED_LIBS glib-2.0 lcm lcmtypes_${POD_NAME})
pods_install_pkg_config_file(${PROJECT_NAME}
    LIBS -lbot2-core -lm
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
