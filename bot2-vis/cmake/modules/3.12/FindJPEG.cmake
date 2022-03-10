# CMake - Cross Platform Makefile Generator
# Copyright 2000-2018 Kitware, Inc. and Contributors
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# * Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
# * Neither the name of Kitware, Inc. nor the names of Contributors
#   may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#.rst:
# FindJPEG
# --------
#
# Find the JPEG library (libjpeg)
#
# Imported targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the following :prop_tgt:`IMPORTED` targets:
#
# ``JPEG::JPEG``
#   The JPEG library, if found.
#
# Result variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``JPEG_FOUND``
#   If false, do not try to use JPEG.
# ``JPEG_INCLUDE_DIRS``
#   where to find jpeglib.h, etc.
# ``JPEG_LIBRARIES``
#   the libraries needed to use JPEG.
# ``JPEG_VERSION``
#   the version of the JPEG library found
#
# Cache variables
# ^^^^^^^^^^^^^^^
#
# The following cache variables may also be set:
#
# ``JPEG_INCLUDE_DIRS``
#   where to find jpeglib.h, etc.
# ``JPEG_LIBRARY_RELEASE``
#   where to find the JPEG library (optimized).
# ``JPEG_LIBRARY_DEBUG``
#   where to find the JPEG library (debug).
#
# Obsolete variables
# ^^^^^^^^^^^^^^^^^^
#
# ``JPEG_INCLUDE_DIR``
#   where to find jpeglib.h, etc. (same as JPEG_INCLUDE_DIRS)
# ``JPEG_LIBRARY``
#   where to find the JPEG library.

find_path(JPEG_INCLUDE_DIR jpeglib.h)

set(jpeg_names ${JPEG_NAMES} jpeg jpeg-static libjpeg libjpeg-static)
foreach(name ${jpeg_names})
  list(APPEND jpeg_names_debug "${name}d")
endforeach()

if(NOT JPEG_LIBRARY)
  find_library(JPEG_LIBRARY_RELEASE NAMES ${jpeg_names})
  find_library(JPEG_LIBRARY_DEBUG NAMES ${jpeg_names_debug})
  include(SelectLibraryConfigurations)
  select_library_configurations(JPEG)
  mark_as_advanced(JPEG_LIBRARY_RELEASE JPEG_LIBRARY_DEBUG)
endif()
unset(jpeg_names)
unset(jpeg_names_debug)

if(JPEG_INCLUDE_DIR AND EXISTS "${JPEG_INCLUDE_DIR}/jpeglib.h")
  file(STRINGS "${JPEG_INCLUDE_DIR}/jpeglib.h"
    jpeg_lib_version REGEX "^#define[\t ]+JPEG_LIB_VERSION[\t ]+.*")

  if (NOT jpeg_lib_version)
    # libjpeg-turbo sticks JPEG_LIB_VERSION in jconfig.h
    find_path(jconfig_dir jconfig.h)
    if (jconfig_dir)
      file(STRINGS "${jconfig_dir}/jconfig.h"
        jpeg_lib_version REGEX "^#define[\t ]+JPEG_LIB_VERSION[\t ]+.*")
    endif()
    unset(jconfig_dir)
  endif()

  string(REGEX REPLACE "^#define[\t ]+JPEG_LIB_VERSION[\t ]+([0-9]+).*"
    "\\1" JPEG_VERSION "${jpeg_lib_version}")
  unset(jpeg_lib_version)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JPEG
  REQUIRED_VARS JPEG_LIBRARY JPEG_INCLUDE_DIR
  VERSION_VAR JPEG_VERSION)

if(JPEG_FOUND)
  set(JPEG_LIBRARIES ${JPEG_LIBRARY})
  set(JPEG_INCLUDE_DIRS "${JPEG_INCLUDE_DIR}")

  if(NOT TARGET JPEG::JPEG)
    add_library(JPEG::JPEG UNKNOWN IMPORTED)
    if(JPEG_INCLUDE_DIRS)
      set_target_properties(JPEG::JPEG PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${JPEG_INCLUDE_DIRS}")
    endif()
    if(EXISTS "${JPEG_LIBRARY}")
      set_target_properties(JPEG::JPEG PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${JPEG_LIBRARY}")
    endif()
    if(EXISTS "${JPEG_LIBRARY_RELEASE}")
      set_property(TARGET JPEG::JPEG APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(JPEG::JPEG PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
        IMPORTED_LOCATION_RELEASE "${JPEG_LIBRARY_RELEASE}")
    endif()
    if(EXISTS "${JPEG_LIBRARY_DEBUG}")
      set_property(TARGET JPEG::JPEG APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(JPEG::JPEG PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
        IMPORTED_LOCATION_DEBUG "${JPEG_LIBRARY_DEBUG}")
    endif()
  endif()
endif()

mark_as_advanced(JPEG_LIBRARY JPEG_INCLUDE_DIR)
