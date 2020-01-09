cmake_minimum_required(VERSION 3.10)
project(bot2-frames LANGUAGES C CXX VERSION 0.0.1)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")

include(CMakePackageConfigHelpers)
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

# pull in the pods macros. See cmake/pods.cmake for documentation
set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)
if(NOT BUILD_SHARED_LIBS)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(cmake/pods.cmake)

find_package(Java 1.8 MODULE)
if(JAVA_FOUND)
  include(UseJava)
endif()

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
if(bot2-vis_FOUND)
    set(USE_BOT_VIS ON)
    message(STATUS "Configuring with bot2-vis")
    add_subdirectory(src/renderer)
else()
    set(USE_BOT_VIS OFF)
    message(STATUS "Configuring without bot2-vis")
endif()

set(EXPORT_FILE ${PROJECT_NAME}-targets.cmake)
set(JAVA_EXPORT_FILE ${PROJECT_NAME}-java-targets.cmake)

configure_package_config_file(cmake/${PROJECT_NAME}-config.cmake.in
  ${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
  NO_SET_AND_CHECK_MACRO
  INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
)

write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake
  COMPATIBILITY AnyNewerVersion
)

configure_file(cmake/modules/FindGLib2.cmake
    ${PROJECT_BINARY_DIR}/modules/FindGLib2.cmake COPYONLY)
configure_file(cmake/modules/FindM.cmake
    ${PROJECT_BINARY_DIR}/modules/FindM.cmake COPYONLY)

# Install FindGLib2.cmake and FindM.cmake which are not standard CMake files
install(FILES cmake/modules/FindGLib2.cmake cmake/modules/FindM.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules
)
# Install FindPkgConfig.cmake which does not import target
# in the CMake 3 style before CMake 3.6.
install(FILES cmake/modules/3.6/FindPkgConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.6)
# Install FindOpenGL.cmake which does not import target
# in the CMake 3 style before CMake 3.8.
install(FILES cmake/modules/3.10/FindOpenGL.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.10)

# Exported targets for build directory
export(EXPORT ${PROJECT_NAME}
  NAMESPACE ${NAMESPACE}
  FILE ${PROJECT_BINARY_DIR}/${EXPORT_FILE}
)

if(JAVA_FOUND)
  export_jars(TARGETS lcmtypes_${PROJECT_NAME}-java
    NAMESPACE ${NAMESPACE}
    FILE ${JAVA_EXPORT_FILE}
  )
endif()

configure_package_config_file(cmake/${PROJECT_NAME}-config.cmake.in
  ${PROJECT_NAME}-config-install.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NO_SET_AND_CHECK_MACRO
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-install.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  RENAME ${PROJECT_NAME}-config.cmake
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NAMESPACE ${NAMESPACE}
  FILE ${EXPORT_FILE}
)

if(JAVA_FOUND)
  install_jar_exports(TARGETS lcmtypes_${PROJECT_NAME}-java
    NAMESPACE ${NAMESPACE}
    FILE ${JAVA_EXPORT_FILE}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )
endif()
