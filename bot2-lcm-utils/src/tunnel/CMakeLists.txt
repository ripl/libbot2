set(ldpc_sources
    ldpc/getopt.cpp
    ldpc/ldpc_create_pchk.cpp
    ldpc/ldpc_fec.cpp
    ldpc/ldpc_fec_iterative_decoding.cpp
    ldpc/ldpc_matrix_sparse.cpp
    ldpc/ldpc_rand.cpp
    ldpc/ldpc_scheme.cpp
    ldpc/ldpc_wrapper.cpp
    ldpc/tools.cpp
    )

add_executable(bot-lcm-tunnel
    introspect.c
    lcm_tunnel_params_t.c
    lcm_tunnel_sub_msg_t.c
    lcm_tunnel_udp_msg_t.c
    lcm_tunnel_disconnect_msg_t.c
    ssocket.c
    lcm_tunnel.cpp
    lcm_tunnel_server.cpp
    signal_pipe.c
    lcm_util.c
    ${ldpc_sources}
    )

add_executable(ldpc-wrapper-test
    ldpc/ldpc_wrapper_test.cpp
    ${ldpc_sources}
    )
set_target_properties(bot-lcm-tunnel PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)

target_link_libraries(bot-lcm-tunnel ${LCM_NAMESPACE}lcm GLib2::glib)

pods_install_executables(EXPORT ${PROJECT_NAME} bot-lcm-tunnel)
