find_package(M MODULE REQUIRED)

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
    ssocket.c
    lcm_tunnel.cpp
    lcm_tunnel_server.cpp
    signal_pipe.c
    lcm_util.c
    ${ldpc_sources}
    )
target_link_libraries(bot-lcm-tunnel
  PRIVATE GLib2::glib ${LCM_NAMESPACE}lcm M::M lcmtypes_bot2-lcm-utils
)

add_executable(ldpc-wrapper-test
    ldpc/ldpc_wrapper_test.cpp
    ${ldpc_sources}
    )

install(TARGETS bot-lcm-tunnel
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
