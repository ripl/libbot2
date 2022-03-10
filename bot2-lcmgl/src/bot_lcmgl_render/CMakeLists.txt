set(c_files lcmgl_decode.c)
set(h_files lcmgl_decode.h)
set(REQUIRED_LIBS)

find_package(M MODULE REQUIRED)
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL MODULE REQUIRED)

if(USE_BOT_VIS)
  find_package(bot2-vis CONFIG REQUIRED)
  list(APPEND c_files lcmgl_bot_renderer.c)
  list(APPEND h_files lcmgl_bot_renderer.h)
  list(APPEND REQUIRED_LIBS gtk+-3.0 lcm >= 1.4 zlib bot2-vis)
endif()

add_library(bot2-lcmgl-renderer ${c_files})
if(APPLE)
  target_compile_definitions(bot2-lcmgl-renderer
    PRIVATE GL_SILENCE_DEPRECATION=1
  )
endif()
target_include_directories(bot2-lcmgl-renderer
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(bot2-lcmgl-renderer
  PRIVATE
    GLib2::glib
    GLib2::gobject
    M::M
    OpenGL::GL
    OpenGL::GLU
    libbot2::bot2-core
    lcmtypes_bot2-lcmgl
)
if(USE_BOT_VIS)
  # Use the package PkgConfig to detect GTK+ headers/library files
  find_package(PkgConfig MODULE REQUIRED)
  pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)
  target_compile_definitions(bot2-lcmgl-renderer PRIVATE USE_BOT_VIS)
  target_link_libraries(bot2-lcmgl-renderer
    PUBLIC ${LCM_NAMESPACE}lcm libbot2::bot2-vis
    PRIVATE PkgConfig::GTK3 ZLIB::ZLIB
  )
endif()

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(bot2-lcmgl-renderer PROPERTIES SOVERSION 1)

install(TARGETS bot2-lcmgl-renderer
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

pods_install_headers(${h_files} DESTINATION bot_lcmgl_render)

pods_install_pkg_config_file(bot2-lcmgl-renderer
    LIBS -lbot2-lcmgl-renderer
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
