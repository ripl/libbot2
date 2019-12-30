include_guard(GLOBAL)

include(CheckCSourceCompiles)
include(CMakeCheckCompilerFlagCommonPatterns)

function(check_linker_flag FLAG VAR)
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
