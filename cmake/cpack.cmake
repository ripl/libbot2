
# Default CPack generators
set(CPACK_GENERATOR TGZ STGZ)

# Detect OS type, OS variant and target architecture
if(UNIX)
  if(APPLE)
    set(OS_TYPE macos)
  else()
    set(OS_TYPE linux)
    # Determine distribution
    file(READ /etc/lsb-release lsb_release)
    string(REGEX REPLACE "^((.*ID|NAME)=)([a-zA-Z0-9]*).*" "\\3"  LINUX_DISTRO ${lsb_release})
  endif()
  # Determine architecture
  if(LINUX_DISTRO STREQUAL "Debian" OR LINUX_DISTRO STREQUAL "Ubuntu" OR LINUX_DISTRO STREQUAL "LinuxMint")
    execute_process(COMMAND dpkg --print-architecture
      OUTPUT_VARIABLE MACHINE_ARCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(OS_TYPE_ARCH_SUFFIX ${MACHINE_ARCH})
  else()
    execute_process(COMMAND uname -m
        OUTPUT_VARIABLE MACHINE_ARCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # Set OS type and ARCH suffix
    set(OS_TYPE_ARCH_SUFFIX ${OS_TYPE}-${MACHINE_ARCH})
  endif()
elseif(WIN32)
  set(OS_TYPE win)
  # Determine architecture
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(MACHINE_ARCH 64)
  else()
    set(MACHINE_ARCH "")
  endif()
  # Set OS type and ARCH suffix
  set(OS_TYPE_ARCH_SUFFIX ${OS_TYPE}${MACHINE_ARCH})
endif()

include(InstallRequiredSystemLibraries)
# Package release version
set(PACKAGE_RELEASE_VERSION 1)
set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};.;libbot2;ALL;/")


set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/packages)
# Caveat: CMAKE_INSTALL_PREFIX and CPACK_PACKAGING_INSTALL_PREFIX have to match because python
# scripts generated at compile-time hardcode paths.
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Set of libraries, tools, and algorithms that are designed to facilitate robotics research")
set(CPACK_PACKAGE_VENDOR "Kitware, Inc.")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}-${PACKAGE_RELEASE_VERSION}_${OS_TYPE_ARCH_SUFFIX})
set(CPACK_STRIP_FILES TRUE)
set(CPACK_SOURCE_STRIP_FILES FALSE)

if(NOT DEFINED CPACK_LIBPNG_VERSION)
  set(CPACK_LIBPNG_VERSION 12-0)
endif()
if(PACKAGE_OPENGL)
  set(CPACK_OPENGL libopengl0, )
endif()
# Debian specific
set(CPACK_DEBIAN_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${MACHINE_ARCH})
set(CPACK_DEBIAN_PACKAGE_RELEASE ${PACKAGE_RELEASE_VERSION})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "lcm (>=1.3.95), freeglut3, ${CPACK_OPENGL} libc6, libgcc1, libglib2.0-0, libglu1-mesa, libgtk2.0-0, libice6, libjpeg8, libpng${CPACK_LIBPNG_VERSION}, libsm6, libstdc++6, libx11-6, libxau6, libxcb1, libxdmcp6, libxext6, libxmu6, libxt6, openjdk-8-jre, python, python-gtk2, python-numpy, python-scipy, zlib1g")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/RobotLocomotion/libbot2")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Francois Budin <francois.budin@kitware.com>")

include(CPack)
