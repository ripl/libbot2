cmake_minimum_required(VERSION 3.10)
project(bot2-vis LANGUAGES C CXX VERSION 0.0.1)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")
if(CMAKE_VERSION VERSION_LESS 3.12)
  list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules/3.12")
endif()
if(CMAKE_VERSION VERSION_LESS 3.14)
  list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules/3.14")
endif()

include(GNUInstallDirs)

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
check_c_compiler_flag(-fstack-protector
  C_COMPILER_SUPPORTS_FSTACK_PROTECTOR
)
check_c_compiler_flag(-fstack-protector-strong
  C_COMPILER_SUPPORTS_FSTACK_PROTECTOR_STRONG
)
check_c_compiler_flag(-Wall
  C_COMPILER_SUPPORTS_WALL
)
check_c_compiler_flag(-Werror=format-security
  C_COMPILER_SUPPORTS_WERROR_FORMAT_SECURITY
)
check_c_compiler_flag(-Wno-unused
  C_COMPILER_SUPPORTS_WNO_UNUSED
)

if(C_COMPILER_SUPPORTS_D_FORTIFY_SOURCE_2)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FORTIFY_SOURCE=2")
endif()
if(C_COMPILER_SUPPORTS_FSTACK_PROTECTOR_STRONG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-strong")
elseif(C_COMPILER_SUPPORTS_FSTACK_PROTECTOR)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector")
endif()
if(C_COMPILER_SUPPORTS_WALL)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()
if(C_COMPILER_SUPPORTS_WERROR_FORMAT_SECURITY)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=format-security")
endif()
if(C_COMPILER_SUPPORTS_WNO_UNUSED)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused")
endif()

include(CheckCXXCompilerFlag)

check_cxx_compiler_flag(-D_FORTIFY_SOURCE=2
  CXX_COMPILER_SUPPORTS_D_FORTIFY_SOURCE_2
)
check_cxx_compiler_flag(-fstack-protector
  CXX_COMPILER_SUPPORTS_FSTACK_PROTECTOR
)
check_cxx_compiler_flag(-fstack-protector-strong
  CXX_COMPILER_SUPPORTS_FSTACK_PROTECTOR_STRONG
)
check_cxx_compiler_flag(-Wall
  CXX_COMPILER_SUPPORTS_WALL
)
check_cxx_compiler_flag(-Werror=format-security
  CXX_COMPILER_SUPPORTS_WERROR_FORMAT_SECURITY
)
check_cxx_compiler_flag(-Wno-unused
  CXX_COMPILER_SUPPORTS_WNO_UNUSED
)

if(CXX_COMPILER_SUPPORTS_D_FORTIFY_SOURCE_2)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
endif()
if(CXX_COMPILER_SUPPORTS_FSTACK_PROTECTOR_STRONG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-strong")
elseif(CXX_COMPILER_SUPPORTS_FSTACK_PROTECTOR)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector")
endif()
if(CXX_COMPILER_SUPPORTS_WALL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()
if(CXX_COMPILER_SUPPORTS_WERROR_FORMAT_SECURITY)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=format-security")
endif()
if(CXX_COMPILER_SUPPORTS_WNO_UNUSED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused")
endif()

include(CheckLinkerFlag)

check_linker_flag(-Wl,-Bsymbolic-functions LINKER_SUPPORTS_BSYMBOLIC_FUNCTIONS)
check_linker_flag(-Wl,-z,defs LINKER_SUPPORTS_Z_DEFS)
check_linker_flag(-Wl,-z,now LINKER_SUPPORTS_Z_NOW)
check_linker_flag(-Wl,-z,relro LINKER_SUPPORTS_Z_RELRO)

if(LINKER_SUPPORTS_BSYMBOLIC_FUNCTIONS)
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bsymbolic-functions"
  )
  set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-Bsymbolic-functions"
  )
endif()
if(LINKER_SUPPORTS_Z_DEFS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,defs")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")
endif()
if(LINKER_SUPPORTS_Z_NOW)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,now")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,now")
endif()
if(LINKER_SUPPORTS_Z_RELRO)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,relro")
endif()

set(POD_NAME ${PROJECT_NAME})
set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)
if(NOT BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(cmake/pods.cmake)

find_package(GLib2 2.32 MODULE REQUIRED COMPONENTS glib gobject)
find_package(M MODULE REQUIRED)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig MODULE REQUIRED)
pkg_check_modules(GDK3 REQUIRED IMPORTED_TARGET gdk-3.0)
pkg_check_modules(GTK3 REQUIRED IMPORTED_TARGET gtk+-3.0)

find_package(lcm 1.4 CONFIG REQUIRED)
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL MODULE REQUIRED)
find_package(GLUT MODULE REQUIRED)

add_subdirectory(src/bot_vis)
add_subdirectory(src/testers)
add_subdirectory(src/rwx-viewer)
add_subdirectory(src/wavefront-viewer)

# bot-ppmsgz script
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/bot-ppmsgz DESTINATION ${CMAKE_INSTALL_BINDIR})

set(EXPORT_FILE ${PROJECT_NAME}-targets.cmake)
configure_file(cmake/${PROJECT_NAME}-config.cmake.in
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake" @ONLY)

configure_file(cmake/modules/FindGLib2.cmake
    ${PROJECT_BINARY_DIR}/modules/FindGLib2.cmake COPYONLY)
configure_file(cmake/modules/FindM.cmake
    ${PROJECT_BINARY_DIR}/modules/FindM.cmake COPYONLY)
if(CMAKE_VERSION VERSION_LESS 3.12)
  configure_file(cmake/modules/3.12/FindJPEG.cmake
      ${PROJECT_BINARY_DIR}/modules/3.12/FindJPEG.cmake COPYONLY)
endif()
if(CMAKE_VERSION VERSION_LESS 3.14)
  configure_file(cmake/modules/3.14/FindX11.cmake
      ${PROJECT_BINARY_DIR}/modules/3.14/FindX11.cmake COPYONLY)
endif()

# Install FindGLib2.cmake and FindM.cmake which are not standard CMake files
install(FILES cmake/modules/FindGLib2.cmake cmake/modules/FindM.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules
)
# Install FindPNG.cmake which does not import target
# in the CMake 3 style before CMake 3.5.
install(FILES cmake/modules/3.5/FindPNG.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.5)
# Install FindPkgConfig.cmake which does not import target
# in the CMake 3 style before CMake 3.6.
install(FILES cmake/modules/3.6/FindPkgConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.6)
# Install FindOpenGL.cmake which does not import target
# in the CMake 3 style before CMake 3.8.
install(FILES cmake/modules/3.10/FindOpenGL.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.10)
# Install FindJPEG.cmake which does not import target
# in the CMake 3 style before CMake 3.12.
install(FILES cmake/modules/3.12/FindJPEG.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.12)
# Install FindX11.cmake which does not import target
# in the CMake 3 style before CMake 3.14.
install(FILES cmake/modules/3.14/FindX11.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.14)

# Exported targets for build directory
export(EXPORT ${PROJECT_NAME}
  NAMESPACE ${NAMESPACE}
  FILE ${PROJECT_BINARY_DIR}/${EXPORT_FILE}
)

pods_install_cmake_config_files(NAMESPACE ${NAMESPACE}
                                EXPORT ${PROJECT_NAME}
                                EXPORT_FILE ${EXPORT_FILE}
                                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
                                CONFIG ${PROJECT_NAME}-config.cmake)
