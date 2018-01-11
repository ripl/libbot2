find_package(PythonInterp REQUIRED)

# install all python packages in the src/ directory
pods_install_python_packages(${CMAKE_CURRENT_SOURCE_DIR}/src)

if(TARGET ${LCM_NAMESPACE}lcm)
  get_target_property(LCM_INCLUDE_DIR ${LCM_NAMESPACE}lcm
    INTERFACE_INCLUDE_DIRECTORIES)
  get_filename_component(LCM_PREFIX "${LCM_INCLUDE_DIR}" DIRECTORY)
  set(LCM_PYTHON_DIR_HINTS "${LCM_PREFIX}/${PYTHON_SITE_PACKAGES_DIR}")
endif()

find_path(LCM_PYTHON_DIR NAMES "lcm/__init__.py" HINTS "${LCM_PYTHON_DIR_HINTS}")

if(NOT LCM_PYTHON_DIR)
  message(FATAL_ERROR "Could NOT find lcm-python")
endif()

configure_file(bot-log2mat.in bot-log2mat @ONLY)

install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/bot-log2mat"
  DESTINATION "${EXECUTABLE_INSTALL_PATH}")
