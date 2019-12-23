# Create a shared library libbot2-frames.so with a single source file
add_library(${PROJECT_NAME} bot_frames.c)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    libbot2::bot2-param-client
    libbot2::bot2-core
  PRIVATE
    GLib2::glib
    lcmtypes_bot2-frames
    libbot2::lcmtypes_bot2-core
    ${OPENGL_LIBRARIES}
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 2)

# make the header public
# install it to include/bot2-frames
pods_install_headers(bot_frames.h DESTINATION bot_frames)

# make the library public
pods_install_libraries(EXPORT ${PROJECT_NAME} ${PROJECT_NAME})

# create a pkg-config file for the library, to make it for other software to
# use it.
set(REQUIRED_LIBS bot2-core bot2-param-client lcmtypes_bot2-frames)
pods_install_pkg_config_file(${PROJECT_NAME}
    CFLAGS
    LIBS -lbot2-frames
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
