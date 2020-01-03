find_package(bot2-vis CONFIG REQUIRED)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig MODULE REQUIRED)
pkg_check_modules(GDK3 REQUIRED IMPORTED_TARGET gdk-3.0)
pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)

add_executable(bot-lcmgl-viewer
    main.c
    udp_util.c
    view_menu.c
    )
if(APPLE)
  target_compile_definitions(bot-lcmgl-viewer
    PRIVATE GL_SILENCE_DEPRECATION=1
  )
endif()
target_link_libraries(bot-lcmgl-viewer
 PRIVATE
   GLib2::glib
   GLib2::gobject
   GLUT::GLUT
   ${LCM_NAMESPACE}lcm
   PkgConfig::GDK3
   PkgConfig::GTK3
   libbot2::bot2-core
   libbot2::bot2-vis
   bot2-lcmgl-renderer
)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-lcmgl-viewer)
