# Macros to simplify compliance with the pods build policies.
#
# To enable the macros, add the following lines to CMakeLists.txt:
#   set(POD_NAME <pod-name>)
#   include(cmake/pods.cmake)
#
# If POD_NAME is not set, then the CMake source directory is used as POD_NAME
#
# Next, any of the following macros can be used.  See the individual macro
# definitions in this file for individual documentation.
#
# C/C++
#   pods_install_headers(...)
#   pods_install_libraries(...)
#   pods_install_executables(...)
#
# Python
#   pods_install_python_packages(...)
#
# ----
# File: pods.cmake
# Distributed with pods version: 12.09.21

# pods_install_headers(<header1.h> ... DESTINATION <subdir_name>)
#
# Install a (list) of header files.
#
# Header files will all be installed to include/<subdir_name>
#
# example:
#   add_library(perception detector.h sensor.h)
#   pods_install_headers(detector.h sensor.h DESTINATION perception)
#
function(pods_install_headers)
    list(GET ARGV -2 checkword)
    if(NOT checkword STREQUAL DESTINATION)
        message(FATAL_ERROR "pods_install_headers missing DESTINATION parameter")
    endif()

    list(GET ARGV -1 dest_dir)
    list(REMOVE_AT ARGV -1)
    list(REMOVE_AT ARGV -1)
    #copy the headers to the INCLUDE_OUTPUT_PATH (${CMAKE_BINARY_DIR}/include)
    foreach(header ${ARGV})
        get_filename_component(_header_name ${header} NAME)
        configure_file(${header} ${INCLUDE_OUTPUT_PATH}/${dest_dir}/${_header_name} COPYONLY)
	endforeach()
	#mark them to be installed
	install(FILES ${ARGV} DESTINATION include/${dest_dir})

endfunction()

# pods_install_executables(<executable1> ...)
#
# Install a (list) of executables to bin/
function(pods_install_executables _export _export_name)
    if(NOT "${_export}" STREQUAL "EXPORT")
        message(FATAL_ERROR "pods_install_executables missing EXPORT parameter")
    endif()
    install(TARGETS ${ARGN} EXPORT ${_export_name} RUNTIME DESTINATION bin)
endfunction()

# pods_install_libraries(<library1> ...)
#
# Install a (list) of libraries to lib/
function(pods_install_libraries _export _export_name)
    if(NOT "${_export}" STREQUAL "EXPORT")
        message(FATAL_ERROR "pods_install_executables missing EXPORT parameter")
    endif()
    install(TARGETS ${ARGN} EXPORT ${_export_name} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endfunction()

# pods_install_pkg_config_file(<package-name>
#                              [VERSION <version>]
#                              [DESCRIPTION <description>]
#                              [CFLAGS <cflag> ...]
#                              [LIBS <lflag> ...]
#                              [REQUIRES <required-package-name> ...])
#
# Create and install a pkg-config .pc file.
#
# example:
#    add_library(mylib mylib.c)
#    pods_install_pkg_config_file(mylib LIBS -lmylib REQUIRES glib-2.0)
function(pods_install_pkg_config_file)
    list(GET ARGV 0 pc_name)
    # TODO error check

    set(pc_version 0.0.1)
    set(pc_description ${pc_name})
    set(pc_requires "")
    set(pc_libs "")
    set(pc_cflags "")
    set(pc_fname "${PKG_CONFIG_OUTPUT_PATH}/${pc_name}.pc")

    set(modewords LIBS CFLAGS REQUIRES VERSION DESCRIPTION)
    set(curmode "")

    # parse function arguments and populate pkg-config parameters
    list(REMOVE_AT ARGV 0)
    foreach(word ${ARGV})
        list(FIND modewords ${word} mode_index)
        if(${mode_index} GREATER -1)
            set(curmode ${word})
        elseif(curmode STREQUAL LIBS)
            set(pc_libs "${pc_libs} ${word}")
        elseif(curmode STREQUAL CFLAGS)
            set(pc_cflags "${pc_cflags} ${word}")
        elseif(curmode STREQUAL REQUIRES)
            set(pc_requires "${pc_requires} ${word}")
        elseif(curmode STREQUAL VERSION)
            set(pc_version ${word})
            set(curmode "")
        elseif(curmode STREQUAL DESCRIPTION)
            set(pc_description "${word}")
            set(curmode "")
        else(${mode_index} GREATER -1)
            message("WARNING incorrect use of pods_add_pkg_config (${word})")
            break()
        endif()
    endforeach()

    # write the .pc file out
    file(WRITE ${pc_fname}
        "prefix=${CMAKE_INSTALL_PREFIX}\n"
        "exec_prefix=\${prefix}\n"
        "libdir=\${exec_prefix}/lib\n"
        "includedir=\${prefix}/include\n"
        "\n"
        "Name: ${pc_name}\n"
        "Description: ${pc_description}\n"
        "Requires: ${pc_requires}\n"
        "Version: ${pc_version}\n"
        "Libs: -L\${libdir} ${pc_libs}\n"
        "Cflags: -I\${includedir} ${pc_cflags}\n")

    # mark the .pc file for installation to the lib/pkgconfig directory
    install(FILES ${pc_fname} DESTINATION lib/pkgconfig)
endfunction()

# _pods_install_python_package(<py_src_dir> <py_module_name>)
#
# Internal helper function
# Install python module in <py_src_dir> to lib/pythonX.Y/dist-packages/<py_module_name>,
# where X.Y refers to the current python version (e.g., 2.6)
#
function(_pods_install_python_package py_src_dir py_module_name)
    if(EXISTS "${py_src_dir}/__init__.py")
        #install the single module
        file(GLOB_RECURSE py_files   ${py_src_dir}/*.py)
        foreach(py_file ${py_files})
            file(RELATIVE_PATH __tmp_path ${py_src_dir} ${py_file})
            get_filename_component(__tmp_dir ${__tmp_path} PATH)
            install(FILES ${py_file}
                DESTINATION "${PYTHON_INSTALL_PATH}/${py_module_name}/${__tmp_dir}")
        endforeach()
    else()
        message(FATAL_ERROR "${py_src_dir} is not a python package!\n")
    endif()
endfunction()

# pods_install_python_packages(<src_dir1> ...)
#
# Install python packages to lib/pythonX.Y/dist-packages, where X.Y refers to
# the current python version (e.g., 2.6)
#
# For each <src_dir> pass in, it will do the following:
# If <src_dir> is a python package (it has a __init__.py file) it will be installed
# along with any .py files in subdirectories
#
# Otherwise the script searches for and installs any python packages in <src_dir>
function(pods_install_python_packages py_src_dir)
    get_filename_component(py_src_abs_dir ${py_src_dir} ABSOLUTE)
    if(ARGC GREATER 1)
        #install each module seperately
        foreach(py_module ${ARGV})
            pods_install_python_packages(${py_module})
        endforeach()
    elseif(EXISTS "${py_src_abs_dir}/__init__.py")
        #install the single module by name
        get_filename_component(py_module_name ${py_src_abs_dir} NAME)
        _pods_install_python_package(${py_src_abs_dir} ${py_module_name})
    else()
        # install any packages within the passed in py_src_dir
        set(_installed_a_package FALSE)
        file(GLOB sub-dirs RELATIVE ${py_src_abs_dir} ${py_src_abs_dir}/*)
        foreach(sub-dir ${sub-dirs})
            if(EXISTS "${py_src_abs_dir}/${sub-dir}/__init__.py")
                _pods_install_python_package(${py_src_abs_dir}/${sub-dir} ${sub-dir})
                set(_installed_a_package TRUE)
            endif()
        endforeach()
        if (NOT _installed_a_package)
            message(FATAL_ERROR "${py_src_dir} does not contain any python packages!\n")
        endif()
    endif()
endfunction()

macro(pods_install_cmake_config_files
    _namespace _namespace_value
    _export _export_name
    _export_file _export_file_name
    _destination _destination_folder
    _config _config_name)
  if(NOT "${_namespace}" STREQUAL "NAMESPACE")
      message(FATAL_ERROR "pods_install_cmake_config_file missing NAMESPACE parameter")
  endif()
  if(NOT "${_export}" STREQUAL "EXPORT")
      message(FATAL_ERROR "pods_install_cmake_config_file missing EXPORT parameter")
  endif()
  if(NOT "${_config}" STREQUAL "CONFIG")
      message(FATAL_ERROR "pods_install_cmake_config_file missing CONFIG parameter")
  endif()
  if(NOT "${_export_file}" STREQUAL "EXPORT_FILE")
      message(FATAL_ERROR "pods_install_cmake_config_file missing CONFIG parameter")
  endif()
  if(NOT "${_destination}" STREQUAL "DESTINATION")
      message(FATAL_ERROR "pods_install_cmake_config_file missing CONFIG parameter")
  endif()
  install(FILES ${CMAKE_BINARY_DIR}/${_config_name} DESTINATION lib/cmake/${_export_name})
  install(EXPORT ${_export_name} FILE ${_export_file_name} DESTINATION ${_destination_folder} NAMESPACE ${_namespace_value})
endmacro()

# pods_config_search_paths()
#
# Setup include, linker, and pkg-config paths according to the pods core
# policy.  This macro is automatically invoked, there is no need to do so
# manually.
macro(pods_config_search_paths)
    if(NOT DEFINED __pods_setup)
		#set where files should be output locally
	    set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
	    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
	    set(INCLUDE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/include)
      set(PKG_CONFIG_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/pkgconfig)

		#set where files should be installed to
	    set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
	    set(EXECUTABLE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/bin)
	    set(INCLUDE_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/include)
      set(PKG_CONFIG_INSTALL_PATH ${LIBRARY_INSTALL_PATH}/pkgconfig)

      find_package(PythonInterp 3.6 MODULE REQUIRED)
      execute_process(
        COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils import sysconfig as sc; print(sc.get_python_lib(prefix='', plat_specific=True))"
        OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
      set(PYTHON_INSTALL_PATH
        "${CMAKE_INSTALL_PREFIX}/${PYTHON_SITE_PACKAGES_DIR}")

      set(CMAKE_INSTALL_RPATH "${LIBRARY_INSTALL_PATH}")
      set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

        # hack to force cmake always create install and clean targets
        install(FILES DESTINATION)
        string(RANDOM LENGTH 32 __rand_target__name__)
        add_custom_target(${__rand_target__name__})
        unset(__rand_target__name__)

        set(__pods_setup true)
    endif()
endmacro()

#set the variable POD_NAME to the directory path, and set the cmake PROJECT_NAME
if(NOT POD_NAME)
    get_filename_component(POD_NAME ${CMAKE_SOURCE_DIR} NAME)
    message(STATUS "POD_NAME is not set... Defaulting to directory name: ${POD_NAME}")
endif()
project(${POD_NAME})

#call the function to setup paths
pods_config_search_paths()
