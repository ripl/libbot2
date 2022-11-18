if(NOT JAVA_FOUND)
    message(STATUS "Not building Java LCM-SPY plugins (Can't find Java)")
    return()
endif()

add_jar(lcmspy_plugins_bot2-java
  SOURCES
    src/bot2_spy/ImagePlugin.java
    src/bot2_spy/PlanarLidarPlugin.java
  INCLUDE_JARS lcm-java lcmtypes_${PROJECT_NAME}-java
  OUTPUT_NAME lcmspy_plugins_bot2
)

install_jar(lcmspy_plugins_bot2-java
  DESTINATION ${CMAKE_INSTALL_DATADIR}/java
)
