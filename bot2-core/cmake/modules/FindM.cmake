#[=============================================================================[
This file is part of bot2-core.

bot2-core is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

bot2-core is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with bot2-core. If not, see <https://www.gnu.org/licenses/>.
#]=============================================================================]

#[========================================================================[.rst:
FindM
-----

Finds the ``m`` math library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``M::M``
  The ``m`` math library.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``M_FOUND``
  True if the system has the ``m`` math library.
``M_LIBRARIES``
  Libraries needed to link to the ``m`` math library.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``M_LIBRARY``
  The path to the ``m`` math library.
#]========================================================================]

include(CheckFunctionExists)
include(FindPackageHandleStandardArgs)

set(_M_SAVED_CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET})
set(CMAKE_REQUIRED_QUIET ON)
check_function_exists(cos M_LINK_FLAG_NOT_REQUIRED)
set(CMAKE_REQUIRED_QUIET ${_M_SAVED_CMAKE_REQUIRED_QUIET})
unset(_M_SAVED_CMAKE_REQUIRED_QUIET)

if(M_LINK_FLAG_NOT_REQUIRED)
  set(_M_REQUIRED_VARS M_FOUND)
  set(M_FOUND ON)
  set(M_LIBRARY)
else()
  set(_M_REQUIRED_VARS M_LIBRARY)
  find_library(M_LIBRARY NAMES m)
endif()

find_package_handle_standard_args(M
  REQUIRED_VARS ${_M_REQUIRED_VARS}
)

unset(_M_REQUIRED_VARS)

if(M_FOUND)
  if(M_LIBRARY)
    set(M_LIBRARIES "${M_LIBRARY}")
    if(NOT TARGET M::M)
      add_library(M::M UNKNOWN IMPORTED)
      set_target_properties(M::M PROPERTIES
        IMPORTED_LOCATION "${M_LIBRARY}"
      )
    endif()
    mark_as_advanced(M_LIBRARY)
  else()
    set(M_LIBRARIES)
    if(NOT TARGET M::M)
      add_library(M::M INTERFACE IMPORTED)
    endif()
  endif()
endif()
