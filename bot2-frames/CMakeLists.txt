cmake_minimum_required(VERSION 3.3)

set(JAVA_INSTALL_SUBDIR share/java) # java
macro(configure_import_targets _prop)
  if("${_prop}" STREQUAL "BUILD")
    # BUILD tree configure
    set(JAVA_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(cmake/${POD_NAME}-config.cmake.in
      "${PROJECT_BINARY_DIR}/${POD_NAME}-config.cmake" @ONLY)
  elseif("${_prop}" STREQUAL "INSTALL")
    # INSTALL tree configure
    set(JAVA_INSTALL_DIR \${\${CMAKE_FIND_PACKAGE_NAME}_IMPORT_PREFIX}/${JAVA_INSTALL_SUBDIR})
    configure_file(cmake/${POD_NAME}-config.cmake.in
        "${PROJECT_BINARY_DIR}/cmake/${POD_NAME}-config.cmake" @ONLY)
  else()
    message(FATAL_ERROR "Choose between BUILD and INSTALL")
  endif()
endmacro()

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME bot2-frames)
set(NAMESPACE libbot::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)

include(cmake/pods.cmake)

include(cmake/lcmtypes.cmake)
lcmtypes_build(EXPORT ${POD_NAME})

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")
find_package(OpenGL REQUIRED)
find_package(bot2-param REQUIRED)
find_package(bot2-core REQUIRED)
find_package(lcm REQUIRED)
find_package(GLib2 REQUIRED)

add_subdirectory(src)
add_subdirectory(src/test)

find_package(bot2-vis)
if(NOT bot2-vis_FOUND)
    message("bot2-vis not found.  Not building libbot2 renderer")
else()
    add_subdirectory(src/renderer)
endif()

set(EXPORT_FILE ${POD_NAME}-targets.cmake)
configure_import_targets(BUILD)
configure_import_targets(INSTALL)

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
                                CONFIG cmake/${POD_NAME}-config.cmake)
