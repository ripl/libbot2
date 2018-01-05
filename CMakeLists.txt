cmake_minimum_required(VERSION 3.3)
project(libbot)

# PODs out-of-source build logic
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" OR CMAKE_INSTALL_PREFIX STREQUAL "C:/Program Files/${PROJECT_NAME}")
  find_file(_build_dir build PATHS ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/.. ${PROJECT_SOURCE_DIR}/../.. ${PROJECT_SOURCE_DIR}/../../.. ${PROJECT_SOURCE_DIR}/../../../..)
  if(_build_dir)
      set(CMAKE_INSTALL_PREFIX "${_build_dir}" CACHE PATH "Install location for project" FORCE)
  else()
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/build)
    set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/build)
  endif()
endif()
message(STATUS CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX})

# set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")

find_package(lcm REQUIRED)
find_package(GLib2 REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

# Look for the gdkx.h header to decide whether the local gtk2 install has
# X11 support or not.
find_file(GDKX_HEADER gdkx.h PATH_SUFFIXES gtk-2.0/gdk)
if(GDKX_HEADER)
  set(bot_vis_default ON)
  message(STATUS "gdkx.h found, system appears to have gtk+X11 support")
else()
  set(bot_vis_default OFF)
  message(STATUS "gdkx.h not found, system not appear to have gtk+X11 support")
endif()


option(WITH_BOT_VIS "Build with bot2-vis" ${bot_vis_default})

include(ExternalProject)

# pass thru certain cmake variables.
set(default_cmake_args
  "-DCMAKE_PREFIX_PATH:PATH=${CMAKE_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}"
  "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
  "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
  "-DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}"
  "-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}"
  "-DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}"
  "-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}"
  "-DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}"
  "-DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}"
  "-DCMAKE_STATIC_LINKER_FLAGS:STRING=${CMAKE_STATIC_LINKER_FLAGS}"
  "-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}"
  "-Dlcm_DIR:PATH=${lcm_DIR}"
  )


ExternalProject_Add(bot2-core
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-core
  BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-core
  CMAKE_CACHE_ARGS ${default_cmake_args}
  BUILD_ALWAYS 1)

ExternalProject_Add(bot2-procman
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-procman
  BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-procman
  CMAKE_CACHE_ARGS ${default_cmake_args}
  BUILD_ALWAYS 1)

ExternalProject_Add(bot2-lcm-utils
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcm-utils
  BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcm-utils
  CMAKE_CACHE_ARGS ${default_cmake_args})

ExternalProject_Add(bot2-param
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-param
  BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-param
  CMAKE_CACHE_ARGS ${default_cmake_args}
  DEPENDS bot2-core)

if(WITH_BOT_VIS)

  message(STATUS "Configuring with bot2-vis, modules requiring gtk+x11 will be built.")

  ExternalProject_Add(bot2-vis
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-vis
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-vis
    CMAKE_CACHE_ARGS ${default_cmake_args}
    DEPENDS bot2-core)

  ExternalProject_Add(bot2-lcmgl
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcmgl
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcmgl
    CMAKE_CACHE_ARGS ${default_cmake_args} -DUSE_BOT_VIS:BOOL=${WITH_BOT_VIS}
    DEPENDS bot2-vis)

  ExternalProject_Add(bot2-frames
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-frames
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-frames
    CMAKE_CACHE_ARGS ${default_cmake_args}
    DEPENDS bot2-core bot2-param bot2-vis bot2-lcmgl)

else()

  message(STATUS "Configuring without bot2-vis, modules requiring gtk+x11 will not be built.")
  find_package(X11 REQUIRED )
  find_package(ZLIB REQUIRED)
  find_package(JPEG REQUIRED)
  find_package(PNG REQUIRED)
  find_package(GTK2 REQUIRED)
  find_package(GLUT REQUIRED)

  ExternalProject_Add(bot2-lcmgl
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcmgl
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcmgl
    CMAKE_CACHE_ARGS ${default_cmake_args} -DUSE_BOT_VIS:BOOL=${WITH_BOT_VIS}
    BUILD_ALWAYS 1)

  ExternalProject_Add(bot2-frames
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-frames
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-frames
    CMAKE_CACHE_ARGS ${default_cmake_args}
    DEPENDS bot2-core bot2-param bot2-lcmgl)

endif()

configure_file(cmake/libbot-config.cmake.in
        "${PROJECT_BINARY_DIR}/libbot-config.cmake" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}/libbot-config.cmake" DESTINATION lib/cmake/libbot)
