# Create a shared library libbot2-frames.so with a single source file
add_library(${POD_NAME} bot_frames.c)

target_link_libraries(${POD_NAME}
  PUBLIC
    libbot::bot2-param-client
    libbot::bot2-core
  PRIVATE
    GLib2::glib
    lcmtypes_bot2-frames
    libbot::lcmtypes_bot2-core
    libbot::lcmtypes_bot2-param
    ${OPENGL_LIBRARIES}
)
target_include_directories(${POD_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set_property(TARGET ${POD_NAME} PROPERTY C_STANDARD 99)
# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(${POD_NAME} PROPERTIES SOVERSION 2)

# make the header public
# install it to include/bot2-frames
pods_install_headers(bot_frames.h DESTINATION bot_frames)

# make the library public
pods_install_libraries(EXPORT ${POD_NAME} ${POD_NAME})

# create a pkg-config file for the library, to make it for other software to
# use it.
set(REQUIRED_LIBS bot2-core bot2-param-client lcmtypes_bot2-frames)
pods_install_pkg_config_file(${POD_NAME}
    CFLAGS
    LIBS -lbot2-frames
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
