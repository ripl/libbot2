if(NOT JAVA_FOUND)
   message(STATUS "Java not found... not building lcmgl java bindings")
   return()
endif()

add_jar(${PROJECT_NAME}-java
  SOURCES src/bot_lcmgl/LCMGL.java
  INCLUDE_JARS lcm-java lcmtypes_${PROJECT_NAME}-java
  OUTPUT_NAME ${PROJECT_NAME}
)

install_jar(${PROJECT_NAME}-java
  DESTINATION ${CMAKE_INSTALL_DATADIR}/java
)
