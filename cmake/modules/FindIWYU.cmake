#[=============================================================================[
This file is part of libbot2.

libbot2 is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

libbot2 is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libbot2. If not, see <https://www.gnu.org/licenses/>.
#]=============================================================================]

#[========================================================================[.rst:
FindIWYU
--------

Finds the ``include-what-you-use`` executable.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``IWYU::IWYU``
  The ``include-what-you-use`` executable.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``IWYU_FOUND``
  True if the system has the ``include-what-you-use`` executable.
``IWYU_VERSION``
  The version of the ``include-what-you-use`` executable that was
  found.
``IWYU_CLANG_VERSION``
  The version ``clang`` upon which the `include-what-you-use`` executable
  that was found is based.
``IWYU_EXECUTABLE``
  The path to the ``include-what-you-use`` executable that was found.
``IWYU_FIX_INCLUDES_EXECUTABLE``
  The path to the ``include-what-you-use`` ``fix_includes.py``
  executable, if found.
``IWYU_IWYU_TOOL_EXECUTABLE``
  The path to the ``include-what-you-use`` ``iwyu_tool.py`` executable,
  if found.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``IWYU_EXECUTABLE``
  The path to an ``include-what-you-use`` or ``iwyu`` executable.
#]========================================================================]

find_program(IWYU_EXECUTABLE NAMES include-what-you-use iwyu
  PATHS /usr/local/bin /usr/bin
  DOC "Path to the include-what-you-use executable"
)

set(IWYU_VERSION)
set(IWYU_CLANG_VERSION)

if(IWYU_EXECUTABLE)
  get_filename_component(_IWYU_EXECUTABLE_DIRECTORY "${IWYU_EXECUTABLE}"
    DIRECTORY
  )
  find_program(IWYU_FIX_INCLUDES_EXECUTABLE
    NAMES fix_includes.py fix_includes fix_include
    HINTS "${_IWYU_EXECUTABLE_DIRECTORY}" PATHS /usr/local/bin /usr/bin
    DOC "Path to the include-what-you-use fix_includes.py executable"
  )
  mark_as_advanced(IWYU_FIX_INCLUDES_EXECUTABLE)
  find_program(IWYU_FIX_INCLUDES_EXECUTABLE NAMES iwyu_tool.py iwyu_tool
    HINTS "${_IWYU_EXECUTABLE_DIRECTORY}" PATHS /usr/local/bin /usr/bin
    DOC "Path to the include-what-you-use iwyu_tool.py executable"
  )
  mark_as_advanced(IWYU_IWYU_TOOL_EXECUTABLE)
  unset(_IWYU_EXECUTABLE_DIRECTORY)

  execute_process(COMMAND "${IWYU_EXECUTABLE}" --version
    TIMEOUT 15
    RESULT_VARIABLE _IWYU_VERSION_RESULT_VARIABLE
    OUTPUT_VARIABLE _IWYU_VERSION_OUTPUT_VARIABLE
    ERROR_QUIET
  )
  if(_IWYU_VERSION_RESULT_VARIABLE EQUAL 0)
    string(REGEX MATCH
      "include-what-you-use ([0-9a-zA-Z.\-]+) based on clang version ([0-9a-zA-Z.\-]+)"
      _IWYU_VERSION_REGEX_MATCH_OUTPUT_VARIABLE
      "${_IWYU_VERSION_OUTPUT_VARIABLE}"
    )
    set(IWYU_VERSION "${CMAKE_MATCH_1}")
    set(IWYU_CLANG_VERSION "${CMAKE_MATCH_2}")
    unset(_IWYU_VERSION_REGEX_MATCH_OUTPUT_VARIABLE)
  endif()
  unset(_IWYU_VERSION_RESULT_VARIABLE)
  unset(_IWYU_VERSION_OUTPUT_VARIABLE)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(IWYU
  FOUND_VAR IWYU_FOUND
  REQUIRED_VARS IWYU_EXECUTABLE
  VERSION_VAR IWYU_VERSION
)

if(IWYU_FOUND)
  if(NOT TARGET IWYU::IWYU)
    add_executable(IWYU::IWYU IMPORTED)
    set_target_properties(IWYU::IWYU PROPERTIES
      IMPORTED_LOCATION "${IWYU_EXECUTABLE}"
    )
  endif()
  mark_as_advanced(IWYU_EXECUTABLE)
endif()
