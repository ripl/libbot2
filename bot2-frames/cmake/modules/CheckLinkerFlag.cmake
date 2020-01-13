#[=============================================================================[
This file is part of bot2-frames.

bot2-frames is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your
option) any later version.

bot2-frames is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with bot2-frames. If not, see <https://www.gnu.org/licenses/>.
#]=============================================================================]

#[========================================================================[.rst:
CheckLinkerFlag
---------------

Check whether the linker supports a given flag.

check_linker_flag
^^^^^^^^^^^^^^^^^

.. code-block:: cmake

   check_linker_flag(<flag> <var>)

Check that the ``<flag>`` is accepted by the linker without a diagnostic.
Stores the result in an ``INTERNAL`` cache entry named ``<var>``.

This function temporarily sets the ``CMAKE_REQUIRED_LIBRARIES`` or
``CMAKE_REQUIRED_LINK_OPTIONS`` variable and calls the
``check_c_source_compiles`` macro from the ``CheckCSourceCompiles`` module.
See documentation of that module for a listing of variables that can
otherwise modify the build.

A positive result from this check indicates only that the linker did not
issue a diagnostic message when given the flag. Whether the flag has any
effect or even a specific one is beyond the scope of this module.

Note that since the ``try_compile`` command forwards flags from variables
like ``CMAKE_C_FLAGS``, unknown flags in such variables may cause a false
negative for this check.
#]========================================================================]

include_guard(GLOBAL)

include(CheckCSourceCompiles)
include(CMakeCheckCompilerFlagCommonPatterns)

function(check_linker_flag FLAG VAR)
  _check_linker_flag(-Wl,--fatal-warnings LINKER_SUPPORTS__FATAL_WARNINGS)
  _check_linker_flag(-Wl,-fatal_warnings LINKER_SUPPORTS_FATAL_WARNINGS)

  if(LINKER_SUPPORTS__FATAL_WARNINGS)
    set(FATAL_WARNINGS_FLAG -Wl,--fatal-warnings)
  elseif(LINKER_SUPPORTS_FATAL_WARNINGS)
    set(FATAL_WARNINGS_FLAG -Wl,-fatal_warnings)
  else()
    set(FATAL_WARNINGS_FLAG)
  endif()

  _check_linker_flag("${FLAG}" ${VAR} "${FATAL_WARNINGS_FLAG}")

  set(${VAR} "${${VAR}}" PARENT_SCOPE)
endfunction()

function(_check_linker_flag FLAG VAR)
  if(ARGC GREATER 2)
    list(INSERT FLAG 0 "${ARGV2}")
  endif()

  if(CMAKE_VERSION VERSION_LESS 3.14)
    set(CMAKE_REQUIRED_LIBRARIES "${FLAG}")
  else()
    set(CMAKE_REQUIRED_LINK_OPTIONS "${FLAG}")
  endif()

  set(LC_VARS LANG LC_ALL LC_MESSAGES)
  foreach(LC_VAR IN LISTS LC_VARS)
    set(SAVED_ENV_${LC_VAR} "$ENV{${LC_VAR}}")
    set(ENV{${LC_VAR}} C)
  endforeach()

  check_compiler_flag_common_patterns(COMMON_PATTERNS)
  check_c_source_compiles("int main() { return 0; }" ${VAR} ${COMMON_PATTERNS})

  foreach(LC_VAR IN LISTS LC_VARS)
    set(ENV{${LC_VAR}} ${SAVED_ENV_${LC_VAR}})
  endforeach()

  set(${VAR} "${${VAR}}" PARENT_SCOPE)
endfunction()
