cmake_minimum_required(VERSION 3.10)
project(bot2-core LANGUAGES C CXX VERSION 0.0.1)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JAVA_INSTALL_SUBDIR share/java) # java
macro(configure_import_targets _prop)
  if("${_prop}" STREQUAL "BUILD")
    # BUILD tree configure
    set(JAVA_BUILD_SUBDIR /java) # subdirectory for targets built in subdirectory "java"
    set(JAVA_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)
    unset(JAVA_BUILD_SUBDIR) # Not set for install tree configuration: All targets in same folder.
  elseif("${_prop}" STREQUAL "INSTALL")
    # INSTALL tree configure
    set(JAVA_INSTALL_DIR \${\${CMAKE_FIND_PACKAGE_NAME}_IMPORT_PREFIX}/${JAVA_INSTALL_SUBDIR})
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
        "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake" @ONLY)
  else()
    message(FATAL_ERROR "Choose between BUILD and INSTALL")
  endif()
endmacro()

set(POD_NAME ${PROJECT_NAME})
set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)
if(NOT BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(cmake/pods.cmake)

include(cmake/lcmtypes.cmake)
lcmtypes_build(EXPORT ${PROJECT_NAME})

set(EXPORT_FILE ${PROJECT_NAME}-targets.cmake)

add_subdirectory(src/bot_core)
add_subdirectory(java)

configure_import_targets(BUILD)
configure_import_targets(INSTALL)
configure_file(cmake/modules/FindGLib2.cmake
    ${PROJECT_BINARY_DIR}/modules/FindGLib2.cmake COPYONLY)
# Install FindGLib2.cmake which is not a standard CMake file
install(FILES cmake/modules/FindGLib2.cmake DESTINATION lib/cmake/${PROJECT_NAME}/modules)

# Exported targets for build directory
export(EXPORT ${PROJECT_NAME}
       NAMESPACE ${NAMESPACE}
       FILE ${PROJECT_BINARY_DIR}/${EXPORT_FILE}
)

pods_install_cmake_config_files(NAMESPACE ${NAMESPACE}
                                EXPORT ${PROJECT_NAME}
                                EXPORT_FILE ${EXPORT_FILE}
                                DESTINATION lib/cmake/${PROJECT_NAME}
                                CONFIG cmake/${PROJECT_NAME}-config.cmake)
