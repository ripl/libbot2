cmake_minimum_required(VERSION 3.10)
project(bot2-lcmgl LANGUAGES C CXX VERSION 0.0.1)

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
check_c_compiler_flag(-fno-common
  C_COMPILER_SUPPORTS_FNO_COMMON
)
check_c_compiler_flag(-fsanitize=safe-stack
  C_COMPILER_SUPPORTS_FSANITIZE_SAFE_STACK
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
check_c_compiler_flag(-Wformat-security
  C_COMPILER_SUPPORTS_WFORMAT_SECURITY
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
if(C_COMPILER_SUPPORTS_FNO_COMMON)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common")
endif()
if(C_COMPILER_SUPPORTS_FSTACK_PROTECTOR_STRONG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-strong")
elseif(C_COMPILER_SUPPORTS_FSTACK_PROTECTOR)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector")
endif()
if(C_COMPILER_SUPPORTS_WALL)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()
if(C_COMPILER_SUPPORTS_WFORMAT_SECURITY)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wformat-security")
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
check_cxx_compiler_flag(-fno-common
  CXX_COMPILER_SUPPORTS_FNO_COMMON
)
check_cxx_compiler_flag(-fsanitize=safe-stack
  CXX_COMPILER_SUPPORTS_FSANITIZE_SAFE_STACK
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
check_cxx_compiler_flag(-Wformat-security
  CXX_COMPILER_SUPPORTS_WFORMAT_SECURITY
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
if(CXX_COMPILER_SUPPORTS_FNO_COMMON)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-common")
endif()
if(CXX_COMPILER_SUPPORTS_FSTACK_PROTECTOR_STRONG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-strong")
elseif(CXX_COMPILER_SUPPORTS_FSTACK_PROTECTOR)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector")
endif()
if(CXX_COMPILER_SUPPORTS_WALL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()
if(CXX_COMPILER_SUPPORTS_WFORMAT_SECURITY)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-security")
endif()
if(CXX_COMPILER_SUPPORTS_WERROR_FORMAT_SECURITY)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=format-security")
endif()
if(CXX_COMPILER_SUPPORTS_WNO_UNUSED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(CheckLinkerFlag)

check_linker_flag(-Wl,-Bsymbolic-functions LINKER_SUPPORTS_BSYMBOLIC_FUNCTIONS)
check_linker_flag(-fsanitize=safe-stack LINKER_SUPPORTS_FSANITIZE_SAFE_STACK)
check_linker_flag(-Wl,-z,defs LINKER_SUPPORTS_Z_DEFS)
check_linker_flag(-Wl,-z,noexecheap LINKER_SUPPORTS_Z_NOEXECHEAP)
check_linker_flag(-Wl,-z,noexecstack LINKER_SUPPORTS_Z_NOEXECSTACK)
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
if(C_COMPILER_SUPPORTS_FSANITIZE_SAFE_STACK AND
  CXX_COMPILER_SUPPORTS_FSANITIZE_SAFE_STACK AND
  LINKER_SUPPORTS_FSANITIZE_SAFE_STACK
)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=safe-stack")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=safe-stack")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=safe-stack")
endif()
if(LINKER_SUPPORTS_Z_DEFS)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,defs")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")
endif()
if(LINKER_SUPPORTS_Z_NOEXECHEAP)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,noexecheap")
  set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecheap"
  )
endif()
if(LINKER_SUPPORTS_Z_NOEXECSTACK)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,noexecstack")
  set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,noexecstack"
  )
endif()
if(LINKER_SUPPORTS_Z_NOW)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,now")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,now")
endif()
if(LINKER_SUPPORTS_Z_RELRO)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,relro")
endif()

set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
set(CMAKE_INSTALL_NAME_DIR @rpath)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

file(RELATIVE_PATH BINDIR_RELATIVE_TO_INSTALLDIR
  "${CMAKE_INSTALL_FULL_BINDIR}"
  "${CMAKE_INSTALL_FULL_LIBDIR}"
)

if(APPLE)
  list(APPEND CMAKE_INSTALL_RPATH
    "@executable_path/${BINDIR_RELATIVE_TO_INSTALLDIR}"
  )
else()
  list(APPEND CMAKE_INSTALL_RPATH
    "$ORIGIN/${BINDIR_RELATIVE_TO_INSTALLDIR}"
  )
endif()

set(NAMESPACE libbot2::)

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" OFF)

include(cmake/pods.cmake)

find_package(Java 1.8 MODULE)
if(JAVA_FOUND)
  include(UseJava)
endif()

include(cmake/lcmtypes.cmake)
lcmtypes_build(EXPORT ${PROJECT_NAME})

find_package(bot2-core CONFIG REQUIRED)
find_package(GLib2 2.32 MODULE REQUIRED COMPONENTS glib gobject)
find_package(lcm 1.4 CONFIG REQUIRED)
find_package(ZLIB MODULE REQUIRED)

option(USE_BOT_VIS "Build bot2-lcmgl with bot2-vis dependency." ON)

add_subdirectory(src/bot_lcmgl_client)
add_subdirectory(src/bot_lcmgl_render)

if(USE_BOT_VIS)
  message(STATUS "Configuring with bot2-vis")
  add_subdirectory(src/lcmgl-viewer)
else()
  message(STATUS "Configuring without bot2-vis")
endif()
add_subdirectory(java)
add_subdirectory(python)

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

# FindGLib2.cmake and FindM.cmake are not standard CMake files.
install(FILES cmake/modules/FindGLib2.cmake cmake/modules/FindM.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules
)

# FindPkgConfig.cmake does not import a target before CMake 3.6.
install(FILES cmake/modules/3.6/FindPkgConfig.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.6
)

# CMakeFindDependencyMacro.cmake does not forward all arguments through to
# find_package before CMake 3.9.
install(FILES cmake/modules/3.9/CMakeFindDependencyMacro.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.9
)

# FindOpenGL.cmake does not import a target before CMake 3.8.
install(FILES cmake/modules/3.10/FindOpenGL.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.10
)

# Exported targets for build directory
export(EXPORT ${PROJECT_NAME}
  NAMESPACE ${NAMESPACE}
  FILE ${PROJECT_BINARY_DIR}/${EXPORT_FILE}
)

if(JAVA_FOUND)
  export_jars(TARGETS ${PROJECT_NAME}-java lcmtypes_${PROJECT_NAME}-java
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
  install_jar_exports(TARGETS ${PROJECT_NAME}-java lcmtypes_${PROJECT_NAME}-java
    NAMESPACE ${NAMESPACE}
    FILE ${JAVA_EXPORT_FILE}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  )
endif()
