# Create an executable program coord-frames-test
add_executable(coord-frames-test coord_frames_test.c)

target_link_libraries(coord-frames-test ${POD_NAME} libbot::bot2-param-client)
set_property(TARGET coord-frames-test PROPERTY C_STANDARD 99)
# make executable public
#pods_install_executables(coord-frames-test)
