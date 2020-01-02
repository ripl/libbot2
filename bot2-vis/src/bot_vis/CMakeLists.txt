file(GLOB c_files *.c)
file(GLOB h_files *.h)

# Keep the GLM library out of the public API
list(REMOVE_ITEM h_files glm.h)

find_package(bot2-core CONFIG REQUIRED)
find_package(JPEG MODULE REQUIRED)
find_package(PNG MODULE REQUIRED)
find_package(Threads MODULE REQUIRED)
find_package(X11 MODULE REQUIRED)
find_package(ZLIB MODULE REQUIRED)

add_library(bot2-vis ${c_files})
if(APPLE)
  target_compile_definitions(bot2-vis
     PRIVATE GL_SILENCE_DEPRECATION=1
  )
endif()
target_include_directories(bot2-vis
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
    ${GTK3_INCLUDE_DIRS}
  PRIVATE ${GDK3_INCLUDE_DIRS}
)
target_link_libraries(bot2-vis
  PUBLIC
    GLib2::glib
    GLib2::gobject
    ${GTK3_LIBRARIES}
    OpenGL::GL
    ZLIB::ZLIB
  PRIVATE
    ${GDK3_LIBRARIES}
    GLUT::GLUT
    JPEG::JPEG
    M::M
    OpenGL::GLU
    PNG::PNG
    Threads::Threads
    X11::X11
    X11::Xutil
    libbot2::bot2-core
)
if(NOT APPLE)
  target_link_libraries(bot2-vis
    PRIVATE OpenGL::GLX
  )
endif()

# set the library API version.  Increment this every time the public API
# changes.
set_target_properties(bot2-vis PROPERTIES SOVERSION 1)

pods_install_libraries(EXPORT ${PROJECT_NAME} ${PROJECT_NAME})

pods_install_headers(${h_files} DESTINATION bot_vis)

set(REQUIRED_LIBS gl glib-2.0 >= 2.32 gobject-2.0 >= 2.32 gtk+-3.0 lcm >= 1.4 zlib bot2-core)
pods_install_pkg_config_file(${PROJECT_NAME}
    LIBS -lbot2-vis
    REQUIRES ${REQUIRED_LIBS}
    VERSION 0.0.1)
