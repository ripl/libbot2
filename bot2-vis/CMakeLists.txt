cmake_minimum_required(VERSION 3.3)

set(POD_NAME bot2-vis)
set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)
if(NOT BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")
include(cmake/pods.cmake)

find_package(GLib2 REQUIRED)
find_package(GTK2 REQUIRED)
find_package(lcm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_subdirectory(src/bot_vis)
add_subdirectory(src/testers)
add_subdirectory(src/rwx-viewer)
add_subdirectory(src/wavefront-viewer)

# bot-ppmsgz script
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/bot-ppmsgz DESTINATION bin)

set(EXPORT_FILE ${POD_NAME}-targets.cmake)
configure_file(cmake/${POD_NAME}-config.cmake.in
    "${PROJECT_BINARY_DIR}/${POD_NAME}-config.cmake" @ONLY)
configure_file(cmake/modules/FindGLib2.cmake
    ${PROJECT_BINARY_DIR}/modules/FindGLib2.cmake COPYONLY)
configure_file(cmake/modules/FindOpenGL.cmake
    ${PROJECT_BINARY_DIR}/modules/FindOpenGL.cmake COPYONLY)

# Install FindGLib2.cmake which is not a standard CMake file
install(FILES cmake/modules/FindGLib2.cmake DESTINATION lib/cmake/${POD_NAME}/modules)
# Install FindOpenGL.cmake which does not import target
# in the CMake 3 style before CMake 3.8.
install(FILES cmake/modules/FindOpenGL.cmake DESTINATION lib/cmake/${POD_NAME}/modules)

# Exported targets for build directory
export(EXPORT ${POD_NAME}
  NAMESPACE ${NAMESPACE}
  FILE ${PROJECT_BINARY_DIR}/${EXPORT_FILE}
)

pods_install_cmake_config_files(NAMESPACE ${NAMESPACE}
                                EXPORT ${POD_NAME}
                                EXPORT_FILE ${EXPORT_FILE}
                                DESTINATION lib/cmake/${POD_NAME}
                                CONFIG ${POD_NAME}-config.cmake)
