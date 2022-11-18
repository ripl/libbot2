add_executable(gl_drawing_area_tester gl_drawing_area_tester.c)
if(APPLE)
  target_compile_definitions(gl_drawing_area_tester
    PRIVATE GL_SILENCE_DEPRECATION=1
  )
endif()
target_link_libraries(gl_drawing_area_tester
  PRIVATE OpenGL::GL OpenGL::GLU PkgConfig::GDK3 bot2-vis
)

add_executable(gl_image_area_tester gl_image_area_tester.c)
if(APPLE)
  target_compile_definitions(gl_image_area_tester
    PRIVATE GL_SILENCE_DEPRECATION=1
  )
endif()
target_link_libraries(gl_image_area_tester
  PRIVATE PkgConfig::GDK3 bot2-vis
)

add_executable(param_widget_tester param_widget_tester.c)
target_link_libraries(param_widget_tester
  PRIVATE bot2-vis
)
