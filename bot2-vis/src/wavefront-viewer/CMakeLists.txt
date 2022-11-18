add_executable(bot-wavefront-viewer
    main.c
    renderer_wavefront.c)
if(APPLE)
  target_compile_definitions(bot-wavefront-viewer
    PRIVATE GL_SILENCE_DEPRECATION=1
  )
endif()
target_link_libraries(bot-wavefront-viewer
  PRIVATE
    GLib2::glib
    GLib2::gobject
    GLUT::GLUT
    OpenGL::GL
    PkgConfig::GTK3
    bot2-vis
)

install(TARGETS bot-wavefront-viewer
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
