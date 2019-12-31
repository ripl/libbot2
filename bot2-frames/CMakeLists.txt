cmake_minimum_required(VERSION 3.10)
project(bot2-frames LANGUAGES C CXX VERSION 0.0.1)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCCompilerFlag)

check_c_compiler_flag(-D_FORTIFY_SOURCE=2
  C_COMPILER_SUPPORTS_D_FORTIFY_SOURCE_2
)

if(C_COMPILER_SUPPORTS_D_FORTIFY_SOURCE_2)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FORTIFY_SOURCE=2")
endif()

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-D_FORTIFY_SOURCE=2
  CXX_COMPILER_SUPPORTS_D_FORTIFY_SOURCE_2
)

if(CXX_COMPILER_SUPPORTS_D_FORTIFY_SOURCE_2)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
endif()

include(CheckLinkerFlag)

check_linker_flag(-Wl,-z,defs LINKER_SUPPORTS_Z_DEFS)
check_linker_flag(-Wl,-z,now LINKER_SUPPORTS_Z_NOW)

if(LINKER_SUPPORTS_Z_DEFS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,defs")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")
endif()
if(LINKER_SUPPORTS_Z_NOW)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,now")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,now")
endif()

set(JAVA_INSTALL_SUBDIR share/java) # java
macro(configure_import_targets _prop)
  if("${_prop}" STREQUAL "BUILD")
    # BUILD tree configure
    set(JAVA_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
      "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)
  elseif("${_prop}" STREQUAL "INSTALL")
    # INSTALL tree configure
    set(JAVA_INSTALL_DIR \${\${CMAKE_FIND_PACKAGE_NAME}_IMPORT_PREFIX}/${JAVA_INSTALL_SUBDIR})
    configure_file(cmake/${PROJECT_NAME}-config.cmake.in
        "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}-config.cmake" @ONLY)
  else()
    message(FATAL_ERROR "Choose between BUILD and INSTALL")
  endif()
endmacro()

# pull in the pods macros. See cmake/pods.cmake for documentation
set(POD_NAME ${PROJECT_NAME})
set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)
if(NOT BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(cmake/pods.cmake)

include(cmake/lcmtypes.cmake)
lcmtypes_build(EXPORT ${PROJECT_NAME})

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL MODULE REQUIRED)
find_package(bot2-param CONFIG REQUIRED)
find_package(bot2-core CONFIG REQUIRED)
find_package(lcm 1.4 CONFIG REQUIRED)
find_package(GLib2 2.32 MODULE REQUIRED COMPONENTS glib gobject)

add_subdirectory(src)
add_subdirectory(src/test)

find_package(bot2-vis CONFIG)
if(NOT bot2-vis_FOUND)
    message("bot2-vis not found.  Not building libbot2 renderer")
else()
    add_subdirectory(src/renderer)
endif()

set(EXPORT_FILE ${PROJECT_NAME}-targets.cmake)
configure_import_targets(BUILD)
configure_import_targets(INSTALL)

# Install FindOpenGL.cmake which does not import target
# in the CMake 3 style before CMake 3.8.
install(FILES cmake/modules/3.10/FindOpenGL.cmake DESTINATION lib/cmake/${PROJECT_NAME}/modules/3.10)

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
