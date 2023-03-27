find_package(Python COMPONENTS Interpreter)
if(Python_Interpreter_FOUND)
  pods_install_python_packages(${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()
