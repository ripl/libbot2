add_executable(bot-rwx-viewer
    main.c
    renderer_rwx.c)
target_link_libraries(bot-rwx-viewer bot2-vis ${GTK3_LIBRARIES} OpenGL::GL ${GLUT_LIBRARIES})
if(APPLE)
    target_compile_definitions(bot-rwx-viewer PRIVATE GL_SILENCE_DEPRECATION=1)
endif()

pods_install_executables(EXPORT ${PROJECT_NAME} bot-rwx-viewer)
