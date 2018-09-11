FROM afdaniele/lcm:latest

# arguments
ARG INSTALL_DIR=/root/libbot2/build

# environment
ENV LIBBOT2_INSTALL_DIR $INSTALL_DIR

# copy prerequisites list
COPY ./scripts/setup/linux/ubuntu/$UBUNTU_DISTRIB_CODENAME/install_prereqs /root/libbot2/install_prereqs

# install prerequisites
RUN /root/libbot2/install_prereqs \
    # clean the apt cache
    && rm -rf /var/lib/apt/lists/*

# copy source code
COPY ./ /root/libbot2/

# build libbot2
RUN cd /root/libbot2/ && make BUILD_PREFIX=$LIBBOT2_INSTALL_DIR

# publish libbot2
#   TODO: I'd like to install this globally but it does not work for some reason,
#   the CMAKE_INSTALL_PREFIX variable always reverts to /root/libbot2/build/.
ENV PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$LIBBOT2_INSTALL_DIR/lib/pkgconfig/
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBBOT2_INSTALL_DIR/lib/
ENV PATH=$PATH:$LIBBOT2_INSTALL_DIR/bin/
