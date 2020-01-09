cmake_minimum_required(VERSION 3.10)
string(TIMESTAMP timestamp "%Y%m%d" UTC)
project(libbot2
  VERSION 0.0.1.${timestamp}
  DESCRIPTION "Libraries, tools, and algorithms designed to facilitate robotics research"
  LANGUAGES C CXX
)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE STRING
    "Install path prefix, prepended onto install directories" FORCE
  )
endif()

include(GNUInstallDirs)

option(PACKAGE_LIBBOT2 "Set install prefix to package libbot2 on linux" OFF)
if(PACKAGE_LIBBOT2)
  set(CMAKE_INSTALL_PREFIX "/opt/${CMAKE_PROJECT_NAME}/${PROJECT_VERSION}" CACHE PATH "Install location for project" FORCE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
  set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
  set(INSTALL_CMD INSTALL_COMMAND :)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are None Debug Release RelWithDebInfo MinSizeRel"
    FORCE
  )
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    None Debug Release RelWithDebInfo MinSizeRel
  )
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

list(INSERT CMAKE_MODULE_PATH 0 "${PROJECT_SOURCE_DIR}/cmake/modules")

include(CMakePackageConfigHelpers)

find_package(lcm 1.4 CONFIG REQUIRED)
find_package(GLib2 2.32 MODULE REQUIRED)
find_package(Java 1.8 MODULE)
find_package(PkgConfig MODULE REQUIRED)
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL MODULE REQUIRED)
find_package(PythonInterp 3.6 MODULE REQUIRED)

# Look for the gdkx.h header to decide whether the local gtk3 install has
# X11 support or not.
find_file(GDKX_HEADER gdkx.h PATH_SUFFIXES gtk-3.0/gdk)

if(GDKX_HEADER)
  message(STATUS "gdkx.h found, system appears to have GTK+ X11 support")
else()
  message(STATUS "gdkx.h not found, system does not appear to have GTK+ X11 support")
endif()

mark_as_advanced(GDKX_HEADER)

include(CMakeDependentOption)

cmake_dependent_option(WITH_BOT_VIS "Build with bot2-vis" ON GDKX_HEADER OFF)

include(ExternalProject)

set(SubprojectBinary_dir
${CMAKE_SYSTEM_PREFIX_PATH}
${CMAKE_PREFIX_PATH}
${PROJECT_BINARY_DIR}/bot2-core
${PROJECT_BINARY_DIR}/bot2-procman
${PROJECT_BINARY_DIR}/bot2-lcm-utils
${PROJECT_BINARY_DIR}/bot2-param
${PROJECT_BINARY_DIR}/bot2-vis
${PROJECT_BINARY_DIR}/bot2-lcmgl
${PROJECT_BINARY_DIR}/bot2-frames
)
string(REPLACE ";" ":::" SubprojectBinary_dir "${SubprojectBinary_dir}")

# pass thru certain cmake variables.
set(COMMON_CMAKE_ARGS
  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
  -DCMAKE_BUILD_WITH_INSTALL_RPATH:BOOL=${CMAKE_BUILD_WITH_INSTALL_RPATH}
  -DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=${CMAKE_INSTALL_RPATH_USE_LINK_PATH}
  "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
  "-DCMAKE_C_COMPILER_LAUNCHER:FILEPATH=${CMAKE_C_COMPILER_LAUNCHER}"
  "-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}"
  "-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}"
  "-DCMAKE_CXX_COMPILER_LAUNCHER:FILEPATH=${CMAKE_CXX_COMPILER_LAUNCHER}"
  "-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}"
  "-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}"
  "-DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}"
  "-DCMAKE_INSTALL_NAME_DIR:STRING=${CMAKE_INSTALL_NAME_DIR}"
  "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
  "-DCMAKE_INSTALL_RPATH:STRING=${CMAKE_INSTALL_RPATH}"
  "-DCMAKE_PREFIX_PATH:PATH=${SubprojectBinary_dir}"
  "-DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}"
  -DCMAKE_SKIP_INSTALL_RPATH:BOOL=${CMAKE_SKIP_INSTALL_RPATH}
  -DCMAKE_SKIP_RPATH:BOOL=${CMAKE_SKIP_RPATH}
  "-DCMAKE_STATIC_LINKER_FLAGS:STRING=${CMAKE_STATIC_LINKER_FLAGS}"
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
  "-DJava_JAVA_EXECUTABLE:FILEPATH=${Java_JAVA_EXECUTABLE}"
  "-Dlcm_DIR:PATH=${lcm_DIR}"
  "-DPYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}"
  )

option(CLANG_TIDY "Run the clang-tidy tool during the build" OFF)
if(CLANG_TIDY)
  find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
  if(NOT CLANG_TIDY_EXECUTABLE)
    message(FATAL_ERROR
      "clang-tidy was NOT found (missing: CLANG_TIDY_EXECUTABLE)"
    )
  endif()
  mark_as_advanced(CLANG_TIDY_EXECUTABLE)
  set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
endif()
if(CMAKE_C_CLANG_TIDY)
  string(REPLACE ";" ":::" C_CLANG_TIDY "${CMAKE_C_CLANG_TIDY}")
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_C_CLANG_TIDY:STRING=${C_CLANG_TIDY}"
  )
endif()
if(CMAKE_CXX_CLANG_TIDY)
  string(REPLACE ";" ":::" CXX_CLANG_TIDY "${CMAKE_CXX_CLANG_TIDY}")
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_CXX_CLANG_TIDY:STRING=${CXX_CLANG_TIDY}"
  )
endif()

option(CPPCHECK "Run the cppcheck tool during the build" OFF)
if(CPPCHECK)
  find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)
  if(NOT CPPCHECK_EXECUTABLE)
    message(FATAL_ERROR "cppcheck was NOT found (missing: CPPCHECK_EXECUTABLE)")
  endif()
  mark_as_advanced(CPPCHECK_EXECUTABLE)
  set(CMAKE_C_CPPCHECK "${CPPCHECK_EXECUTABLE}")
  set(CMAKE_CXX_CPPCHECK "${CPPCHECK_EXECUTABLE}")
endif()
if(CMAKE_C_CPPCHECK)
  string(REPLACE ";" ":::" C_CPPCHECK "${CMAKE_C_CPPCHECK}")
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_C_CPPCHECK:STRING=${C_CPPCHECK}"
  )
endif()
if(CMAKE_CXX_CPPCHECK)
  string(REPLACE ";" ":::" CXX_CPPCHECK "${CMAKE_CXX_CPPCHECK}")
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_CXX_CPPCHECK:STRING=${CXX_CPPCHECK}"
  )
endif()
option(CPPLINT "Run the cpplint tool during the build" OFF)
if(CPPLINT)
  find_program(CPPLINT_EXECUTABLE NAMES cpplint.py cpplint)
  if(NOT CPPLINT_EXECUTABLE)
    message(FATAL_ERROR "cpplint was NOT found (missing: CPPLINT_EXECUTABLE)")
  endif()
  mark_as_advanced(CPPLINT_EXECUTABLE)
  set(C_CXX_CPPLINT "${CPPLINT_EXECUTABLE}" --extensions=c,cpp,h --quiet)
  set(CMAKE_C_CPPLINT "${C_CXX_CPPLINT}")
  set(CMAKE_CXX_CPPLINT "${C_CXX_CPPLINT}")
endif()
if(CMAKE_C_CPPLINT)
  string(REPLACE ";" ":::" C_CPPLINT "${CMAKE_C_CPPLINT}")
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_C_CPPLINT:STRING=${C_CPPLINT}"
  )
endif()
if(CMAKE_CXX_CPPLINT)
  string(REPLACE ";" ":::" CXX_CPPLINT "${CMAKE_CXX_CPPLINT}")
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_CXX_CPPLINT:STRING=${CXX_CPPLINT}"
  )
endif()

option(INCLUDE_WHAT_YOU_USE
  "Run the include-what-you-use tool during the build" OFF
)
if(INCLUDE_WHAT_YOU_USE)
  find_package(IWYU MODULE REQUIRED)
  set(C_CXX_INCLUDE_WHAT_YOU_USE "${IWYU_EXECUTABLE}" -Xiwyu
    "--mapping_file=${PROJECT_SOURCE_DIR}/scripts/lint/include-what-you-use.imp"
  )
  if(IWYU VERSION_GREATER_EQUAL 0.10)
    list(APPEND C_CXX_INCLUDE_WHAT_YOU_USE -Xiwyu --no_fwd_decls)
  endif()
  set(CMAKE_C_INCLUDE_WHAT_YOU_USE "${C_CXX_INCLUDE_WHAT_YOU_USE}")
  set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "${C_CXX_INCLUDE_WHAT_YOU_USE}")
endif()
if(CMAKE_C_INCLUDE_WHAT_YOU_USE)
  string(REPLACE ";" ":::" C_INCLUDE_WHAT_YOU_USE
    "${CMAKE_C_INCLUDE_WHAT_YOU_USE}"
  )
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_C_INCLUDE_WHAT_YOU_USE:STRING=${C_INCLUDE_WHAT_YOU_USE}"
  )
endif()
if(CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
  string(REPLACE ";" ":::" CXX_INCLUDE_WHAT_YOU_USE
    "${CMAKE_CXX_INCLUDE_WHAT_YOU_USE}"
  )
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_CXX_INCLUDE_WHAT_YOU_USE:STRING=${CXX_INCLUDE_WHAT_YOU_USE}"
  )
endif()

option(LINK_WHAT_YOU_USE
  "Run the ldd tool during the build" OFF
)
if(LINK_WHAT_YOU_USE)
  find_program(LDD_EXECUTABLE NAMES ldd)
  if(NOT LDD_EXECUTABLE)
    message(FATAL_ERROR "ldd was NOT found (missing: LDD_EXECUTABLE)")
  endif()
  mark_as_advanced(LDD_EXECUTABLE)
  include(CheckLinkerFlag)
  check_linker_flag(-Wl,--no-as-needed LINKER_SUPPORTS_NO_AS_NEEDED)
  if(NOT LINKER_SUPPORTS_NO_AS_NEEDED)
    message(FATAL_ERROR "linker does NOT support flag --no-as-needed")
  endif()
  set(CMAKE_LINK_WHAT_YOU_USE ON)
endif()
if(CMAKE_LINK_WHAT_YOU_USE)
  list(APPEND COMMON_CMAKE_ARGS
    "-DCMAKE_LINK_WHAT_YOU_USE:BOOL=${CMAKE_LINK_WHAT_YOU_USE}"
  )
endif()

ExternalProject_Add(bot2-core
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-core
  BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-core
  LIST_SEPARATOR ":::"
  CMAKE_ARGS ${COMMON_CMAKE_ARGS}
  ${INSTALL_CMD}
  BUILD_ALWAYS ON
)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-core;bot2core;Unspecified;/")

option(WITH_BOT_PROCMAN "Build with bot2-procman" ON)

if(WITH_BOT_PROCMAN)
  ExternalProject_Add(bot2-procman
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-procman
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-procman
    LIST_SEPARATOR ":::"
    CMAKE_ARGS ${COMMON_CMAKE_ARGS}
    "-DLCM_PYTHON_DIR_HINTS:PATH=${LCM_PYTHON_DIR_HINTS}"
    ${INSTALL_CMD}
    BUILD_ALWAYS ON
  )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-procman;bot2-procman;ALL;/")
endif()

option(WITH_BOT_LCM_UTILS "Build with lcm-utils" ON)

if(WITH_BOT_LCM_UTILS)
  ExternalProject_Add(bot2-lcm-utils
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcm-utils
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcm-utils
    LIST_SEPARATOR ":::"
    CMAKE_ARGS ${COMMON_CMAKE_ARGS}
    "-DLCM_PYTHON_DIR_HINTS:PATH=${LCM_PYTHON_DIR_HINTS}"
    ${INSTALL_CMD}
    BUILD_ALWAYS ON
  )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-lcm-utils;bot2-lcm-utils;ALL;/")
endif()

option(WITH_BOT_PARAM "Build with bot2-param" ON)

if(WITH_BOT_PARAM)
  ExternalProject_Add(bot2-param
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-param
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-param
    LIST_SEPARATOR ":::"
    CMAKE_ARGS ${COMMON_CMAKE_ARGS}
    ${INSTALL_CMD}
    DEPENDS bot2-core
  )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-param;bot2-param;ALL;/")
endif()

option(WITH_BOT_LCMGL "Build with bot2-lcmgl" ON)

cmake_dependent_option(WITH_BOT_FRAMES "Build with bot2-frames, requires bot2-lcmgl and bot2-param" ON "WITH_BOT_LCMGL AND WITH_BOT_PARAM" OFF)

if(WITH_BOT_VIS)
  message(STATUS "Configuring with bot2-vis, modules requiring GTK+ X11 will be built.")

  ExternalProject_Add(bot2-vis
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-vis
    BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-vis
    CMAKE_ARGS ${COMMON_CMAKE_ARGS}
    LIST_SEPARATOR ":::"
    ${INSTALL_CMD}
    DEPENDS bot2-core
  )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-vis;bot2-vis;ALL;/")

  if(WITH_BOT_LCMGL)
    ExternalProject_Add(bot2-lcmgl
      SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcmgl
      BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcmgl
      LIST_SEPARATOR ":::"
      CMAKE_ARGS ${COMMON_CMAKE_ARGS} -DUSE_BOT_VIS:BOOL=${WITH_BOT_VIS}
      ${INSTALL_CMD}
      DEPENDS bot2-core bot2-vis
    )
    set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-lcmgl;bot2-lcmgl;ALL;/")

    if(WITH_BOT_FRAMES AND WITH_BOT_PARAM)
      ExternalProject_Add(bot2-frames
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-frames
        BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-frames
        LIST_SEPARATOR ":::"
        CMAKE_ARGS ${COMMON_CMAKE_ARGS}
        ${INSTALL_CMD}
        DEPENDS bot2-core bot2-lcmgl bot2-param bot2-vis
      )
      set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-frames;bot2-frames;ALL;/")
    endif()
  endif()
else()
  if(WITH_BOT_LCMGL)
    message(STATUS "Configuring without bot2-vis, modules requiring GTK+ X11 will not be built.")
    find_package(ZLIB MODULE REQUIRED)

    ExternalProject_Add(bot2-lcmgl
      SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-lcmgl
      BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-lcmgl
      CMAKE_ARGS ${COMMON_CMAKE_ARGS} -DUSE_BOT_VIS:BOOL=${WITH_BOT_VIS}
      LIST_SEPARATOR ":::"
      ${INSTALL_CMD}
      DEPENDS bot2-core
    )
    set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-lcmgl;bot2-lcmgl;ALL;/")

    if(WITH_BOT_FRAMES AND WITH_BOT_PARAM)
      ExternalProject_Add(bot2-frames
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/bot2-frames
        BINARY_DIR ${PROJECT_BINARY_DIR}/bot2-frames
        CMAKE_ARGS ${COMMON_CMAKE_ARGS}
        LIST_SEPARATOR ":::"
        ${INSTALL_CMD}
        DEPENDS bot2-core bot2-lcmgl bot2-param
      )
      set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${PROJECT_BINARY_DIR}/bot2-frames;bot2-frames;ALL;/")
    endif()
  endif()
endif()

# CMakeFindDependencyMacro.cmake does not forward all arguments through to
# find_package before CMake 3.9.
install(FILES cmake/modules/3.9/CMakeFindDependencyMacro.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}/modules/3.9
)

configure_package_config_file(cmake/${PROJECT_NAME}-config.cmake.in
  ${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

if(PACKAGE_LIBBOT2)
  include(cmake/cpack.cmake)
endif()
