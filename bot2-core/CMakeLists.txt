cmake_minimum_required(VERSION 3.3)

set(JAVA_INSTALL_SUBDIR share/java) # java
macro(configure_import_targets _prop)
  if("${_prop}" STREQUAL "BUILD")
    # BUILD tree configure
    set(JAVA_BUILD_SUBDIR /java) # subdirectory for targets built in subdirectory "java"
    set(JAVA_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(cmake/${POD_NAME}-config.cmake.in
      "${PROJECT_BINARY_DIR}/${POD_NAME}-config.cmake" @ONLY)
    unset(JAVA_BUILD_SUBDIR) # Not set for install tree configuration: All targets in same folder.
  elseif("${_prop}" STREQUAL "INSTALL")
    # INSTALL tree configure
    set(JAVA_INSTALL_DIR \${\${CMAKE_FIND_PACKAGE_NAME}_IMPORT_PREFIX}/${JAVA_INSTALL_SUBDIR})
    configure_file(cmake/${POD_NAME}-config.cmake.in
        "${PROJECT_BINARY_DIR}/cmake/${POD_NAME}-config.cmake" @ONLY)
  else()
    message(FATAL_ERROR "Choose between BUILD and INSTALL")
  endif()
endmacro()

set(POD_NAME bot2-core)
set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)

include(cmake/pods.cmake)

include(cmake/lcmtypes.cmake)
lcmtypes_build(EXPORT ${POD_NAME})


set(EXPORT_FILE ${POD_NAME}-targets.cmake)

add_subdirectory(src/bot_core)
add_subdirectory(java)

configure_import_targets(BUILD)
configure_import_targets(INSTALL)
configure_file(cmake/modules/FindGLib2.cmake
    ${PROJECT_BINARY_DIR}/modules/FindGLib2.cmake COPYONLY)
# Install FindGLib2.cmake which is not a standard CMake file
install(FILES cmake/modules/FindGLib2.cmake DESTINATION lib/cmake/${POD_NAME}/modules)

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
