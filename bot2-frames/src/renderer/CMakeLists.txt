# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig MODULE REQUIRED)
pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)

# Create a shared library libbot2-frames-renderer.so with a single source file
add_library(bot2-frames-renderers
    coord_frames_renderer.c articulated_body_renderer.c frame_modifier_renderer.c)
if(APPLE)
  target_compile_definitions(bot2-frames-renderers
     PRIVATE GL_SILENCE_DEPRECATION=1
  )
endif()
target_include_directories(bot2-frames-renderers
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(bot2-frames-renderers
  PUBLIC
    libbot2::bot2-param-client
    libbot2::bot2-vis
    bot2-frames
  PRIVATE
    GLib2::glib
    GLib2::gobject
    M::M
    OpenGL::GL
    OpenGL::GLU
    PkgConfig::GTK3
    libbot2::bot2-core
)
# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(bot2-frames-renderers PROPERTIES SOVERSION 1)

# make the header public
# install it to CMAKE_INSTALL_INCLUDE_DIR/bot_frames
pods_install_headers(bot_frames_renderers.h DESTINATION bot_frames)

install(TARGETS bot2-frames-renderers
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# create a pkg-config file for the library, to make it for other software to
# use it.
set(REQUIRED_PACKAGES bot2-frames bot2-param-client bot2-vis)
pods_install_pkg_config_file(bot2-frames-renderers
    CFLAGS
    LIBS -lbot2-frames-renderers
    REQUIRES ${REQUIRED_PACKAGES}
    VERSION 0.0.1)

find_package(GLUT MODULE REQUIRED)
find_package(bot2-lcmgl CONFIG REQUIRED)

#build the test-viewer
add_executable(test-viewer test_viewer.c)
if(APPLE)
  target_compile_definitions(test-viewer
    PRIVATE GL_SILENCE_DEPRECATION=1
  )
endif()
target_link_libraries(test-viewer
  PRIVATE
    GLib2::glib
    GLUT::GLUT
    ${LCM_NAMESPACE}lcm
    PkgConfig::GTK3
    libbot2::bot2-lcmgl-renderer
    libbot2::bot2-vis
    bot2-frames
    bot2-frames-renderers
)
