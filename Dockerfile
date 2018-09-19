FROM afdaniele/lcm:latest

# arguments
ARG INSTALL_DIR=/usr/local

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
WORKDIR /root
RUN cd libbot2/ && BUILD_PREFIX=$LIBBOT2_INSTALL_DIR make
