# .rst:
# FindM
# -----
#
# Finds the ``util`` system utilities library.
#
# Imported Targets
# ^^^^^^^^^^^^^^^^
#
# This module provides the following imported targets, if found:
#
# ``Util::Util``
#   The ``util`` system utilities library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This will define the following variables:
#
# ``UTIL_FOUND``
#   True if the system has the ``util`` system utilities library.
# ``UTIL_LIBRARIES``
#   Libraries needed to link to the ``util`` system utilities library.
#
# Cache Variables
# ^^^^^^^^^^^^^^^
#
# The following cache variables may also be set:
#
# ``UTIL_LIBRARY``
#   The path to the ``util`` system utilities library.

include(FindPackageHandleStandardArgs)

find_library(UTIL_LIBRARY NAMES util)

find_package_handle_standard_args(Util
  REQUIRED_VARS UTIL_LIBRARY
)

if(UTIL_FOUND)
  set(UTIL_LIBRARIES "${UTIL_LIBRARY}")
  if(NOT TARGET Util::Util)
    add_library(Util::Util UNKNOWN IMPORTED)
    set_target_properties(Util::Util PROPERTIES
      IMPORTED_LOCATION "${UTIL_LIBRARY}"
    )
  endif()
  mark_as_advanced(UTIL_LIBRARY)
endif()
