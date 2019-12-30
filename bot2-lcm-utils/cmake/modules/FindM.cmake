# .rst:
# FindM
# -----
#
# Finds the ``m`` math library.
#
# Imported Targets
# ^^^^^^^^^^^^^^^^
#
# This module provides the following imported targets, if found:
#
# ``M::M``
#   The ``m`` math library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This will define the following variables:
#
# ``M_FOUND``
#   True if the system has the ``m`` math library.
# ``M_LIBRARIES``
#   Libraries needed to link to the ``m`` math library.
#
# Cache Variables
# ^^^^^^^^^^^^^^^
#
# The following cache variables may also be set:
#
# ``M_LIBRARY``
#   The path to the ``m`` math library.

include(FindPackageHandleStandardArgs)

find_library(M_LIBRARY NAMES m)

find_package_handle_standard_args(M
  REQUIRED_VARS M_LIBRARY
)

if(M_FOUND)
  set(M_LIBRARIES "${M_LIBRARY}")
  if(NOT TARGET M::M)
    add_library(M::M UNKNOWN IMPORTED)
    set_target_properties(M::M PROPERTIES
      IMPORTED_LOCATION "${M_LIBRARY}"
    )
  endif()
  mark_as_advanced(M_LIBRARY)
endif()
