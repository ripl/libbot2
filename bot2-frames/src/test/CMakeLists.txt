# Create an executable program coord-frames-test
add_executable(coord-frames-test coord_frames_test.c)

target_link_libraries(coord-frames-test
  PRIVATE
     ${LCM_NAMESPACE}lcm
     libbot2::bot2-core
     libbot2::bot2-param-client
     libbot2::lcmtypes_bot2-core
     bot2-frames
)
# make executable public
#pods_install_executables(coord-frames-test)
