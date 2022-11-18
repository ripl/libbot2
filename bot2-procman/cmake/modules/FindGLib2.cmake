#[=============================================================================[
This file is part of bot2-procman.

bot2-procman is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

bot2-procman is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with bot2-procman. If not, see <https://www.gnu.org/licenses/>.
#]=============================================================================]

#[========================================================================[.rst:
FindGLib2
---------

Finds the glib-2.0 library and related libraries.

The following components are supported:

* ``glib``: search for the glib-2.0 library.
* ``gio``: search for the gio-2.0 library.
* ``gmodule``: search for the gmodule-2.0 library.
* ``gobject``: search for  the gobject-2.0 library.
* ``gthread``: search for the gthread-2.0 library.

If no ``COMPONENTS`` are specified, then ``glib`` is assumed.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``GLib2::glib``
  The glib-2.0 library. Target defined if component ``glib`` is found.
``GLib2::gio``
  The gio-2.0 library. Target defined if component ``gio`` is found.
``GLib2::gmodule``
  The gmodule-2.0 library. Target defined if component ``gmodule`` is
  found.
``GLib2::gobject``
  The gobject-2.0 library. Target defined if component ``gobject`` is
  found.
``GLib2::gthread``
  The gthread-2.0 library. Target defined if component ``gthread`` is
  found.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``GLib2_FOUND``
  True if the system has the glib-2.0 library.
``GLib2_VERSION``
  The version of the glib-2.0 library that was found.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``GLIB2_GLIB_INCLUDE_DIR``
  The directory containing ``glib.h``.
``GLIB2_GLIB_LIBRARY``
  The path to the glib-2.0 library.
``GLIB2_GLIBCONFIG_INCLUDE_DIR``
  The directory containing ``glibconfig.h``.
``GLIB2_GIO_INCLUDE_DIR``
  The directory containing ``gio/gio.h``.
``GLIB2_GIO_LIBRARY``
  The path to the gio-2.0 library.
``GLIB2_GMODULE_INCLUDE_DIR``
  The directory containing ``gmodule.h``.
``GLIB2_GMODULE_LIBRARY``
  The path to the gmodule-2.0 library.
``GLIB2_GOBJECT_INCLUDE_DIR``
  The directory containing ``glib-object.h``.
``GLIB2_GOBJECT_LIBRARY``
  The path to the gobject-2.0 library.
``GLIB2_GTHREAD_LIBRARY``
  The path to the gthread-2.0 library.
#]========================================================================]

include(FindPackageHandleStandardArgs)

find_package(PkgConfig QUIET)

#------------------------------------------------------------------------------
function(_glib2_find_include VAR HEADER)
  if(DEFINED ENV{GLIB_PATH})
    list(APPEND CMAKE_PREFIX_PATH $ENV{GLIB_PATH})
  endif()

  set(_paths)
  foreach(_lib ${ARGN})
    get_filename_component(_libpath ${GLIB2_${_lib}_LIBRARY} DIRECTORY)
    list(APPEND _paths ${_libpath})
  endforeach()

  find_path(GLIB2_${VAR}_INCLUDE_DIR ${HEADER}
    PATHS ${_paths}
    PATH_SUFFIXES glib-2.0 glib-2.0/include
  )
  mark_as_advanced(GLIB2_${VAR}_INCLUDE_DIR)
endfunction()

#------------------------------------------------------------------------------
function(_glib2_find_library VAR LIB)
  if(DEFINED ENV{GLIB_PATH})
    list(APPEND CMAKE_PREFIX_PATH $ENV{GLIB_PATH})
  endif()

  pkg_check_modules(PC_GLIB2_${VAR} QUIET ${LIB}-2.0)

  find_library(GLIB2_${VAR}_LIBRARY
    NAMES ${LIB}-2.0 ${LIB}
    PATHS ${PC_GLIB2_${VAR}_LIBRARY_DIRS}
  )
  mark_as_advanced(GLIB2_${VAR}_LIBRARY)

  if(WIN32)
    find_program(GLIB2_${VAR}_RUNTIME NAMES lib${LIB}-2.0-0.dll)
    mark_as_advanced(GLIB2_${VAR}_RUNTIME)
  endif()

  set(GLIB2_${VAR}_VERSION ${PC_GLIB2_${VAR}_VERSION} PARENT_SCOPE)
endfunction()

#------------------------------------------------------------------------------
function(_glib2_add_target TARGET LIBRARY)
  set(_target GLib2::${TARGET})
  if(TARGET ${_target})
    return()
  endif()
  set(GLIB2_${TARGET}_FIND_QUIETLY TRUE)
  set(_deps GLIB2_${LIBRARY}_LIBRARY)
  foreach(_include ${ARGN})
    list(APPEND _deps GLIB2_${_include}_INCLUDE_DIR)
  endforeach()

  find_package_handle_standard_args(GLib2_${TARGET}
    FOUND_VAR GLib2_${TARGET}_FOUND
    REQUIRED_VARS ${_deps}
  )

  if(GLib2_${TARGET}_FOUND)
    set(GLib2_${TARGET}_FOUND TRUE PARENT_SCOPE)

    add_library(${_target} UNKNOWN IMPORTED)
    set_property(TARGET ${_target} APPEND PROPERTY
      IMPORTED_LOCATION ${GLIB2_${LIBRARY}_LIBRARY}
    )
    foreach(_include ${ARGN})
      set_property(TARGET ${_target} APPEND PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${GLIB2_${_include}_INCLUDE_DIR}
      )
    endforeach()
  endif()
endfunction()

###############################################################################

_glib2_find_library(GLIB glib)
_glib2_find_include(GLIB glib.h)
_glib2_find_include(GLIBCONFIG glibconfig.h GLIB)

_glib2_add_target(glib GLIB GLIB GLIBCONFIG)

if(WIN32 AND TARGET GLib2::glib)
  set_property(TARGET GLib2::glib APPEND PROPERTY
    INTERFACE_LINK_LIBRARIES ws2_32 winmm
  )
endif()

foreach(_glib2_component ${GLib2_FIND_COMPONENTS})
  if(_glib2_component STREQUAL "gio")
    _glib2_find_include(GIO gio/gio.h)
    _glib2_find_library(GIO gio)
    _glib2_add_target(gio GIO GIO GMODULE GOBJECT GLIB GLIBCONFIG)
  elseif(_glib2_component STREQUAL "gmodule")
    _glib2_find_include(GMODULE gmodule.h)
    _glib2_find_library(GMODULE gmodule)
    _glib2_add_target(gmodule GMODULE GMODULE GLIB GLIBCONFIG)
  elseif(_glib2_component STREQUAL "gobject")
    _glib2_find_include(GOBJECT glib-object.h)
    _glib2_find_library(GOBJECT gobject)
    _glib2_add_target(gobject GOBJECT GOBJECT GLIB GLIBCONFIG)
  elseif(_glib2_component STREQUAL "gthread")
    _glib2_find_library(GTHREAD gthread)
    _glib2_add_target(gthread GTHREAD GLIB GLIBCONFIG)
  endif()
endforeach()

list(APPEND GLib2_FIND_COMPONENTS glib)
set(GLib2_FIND_REQUIRED_glib TRUE)

find_package_handle_standard_args(GLib2
  REQUIRED_VARS GLIB2_GLIB_LIBRARY
  VERSION_VAR GLIB2_GLIB_VERSION
  HANDLE_COMPONENTS
)
