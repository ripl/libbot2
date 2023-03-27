include_guard(GLOBAL)

# Macros to simplify compliance with the pods build policies.
#
# To enable the macros, add the following lines to CMakeLists.txt:
#   include(cmake/pods.cmake)
#
# Next, any of the following macros can be used.  See the individual macro
# definitions in this file for individual documentation.
#
# C/C++
#   pods_install_headers(...)
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
# Header files will all be installed to ${CMAKE_INSTALL_INCLUDEDIR}/<subdir_name>
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
    #copy the headers to the INCLUDE_OUTPUT_PATH (${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR})
    foreach(header ${ARGV})
        get_filename_component(_header_name ${header} NAME)
        configure_file(${header} ${INCLUDE_OUTPUT_PATH}/${dest_dir}/${_header_name} COPYONLY)
	endforeach()
	#mark them to be installed
	install(FILES ${ARGV} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${dest_dir})

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
    set(pc_fname "${CMAKE_CURRENT_BINARY_DIR}/${pc_name}.pc")

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
        "libdir=\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}\n"
        "includedir=\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}\n"
        "\n"
        "Name: ${pc_name}\n"
        "Description: ${pc_description}\n"
        "Requires: ${pc_requires}\n"
        "Version: ${pc_version}\n"
        "Libs: -L\${libdir} ${pc_libs}\n"
        "Cflags: -I\${includedir} ${pc_cflags}\n")

    # mark the .pc file for installation to the lib/pkgconfig directory
    install(FILES ${pc_fname} DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
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
    if(NOT PYTHON_INSTALL_PATH)
        message(FATAL_ERROR "PYTHON_INSTALL_PATH is NOT set")
    endif()
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

# Set where headers should be output locally
set(INCLUDE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_INCLUDEDIR})

find_package(Python COMPONENTS Interpreter)
if(Python_Interpreter_FOUND)
  execute_process(
    COMMAND "${Python_EXECUTABLE}" -c "from sysconfig import get_path; from os.path import sep; print(get_path('platlib').replace(get_path('data') + sep, ''))"
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(PYTHON_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/${PYTHON_SITE_PACKAGES_DIR}")
else()
  set(PYTHON_INSTALL_PATH)
endif()
