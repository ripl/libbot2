# Copyright (C) 2018 Massachusetts Institute of Technology.
#
# This program is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <https://www.gnu.org/licenses/>.

FROM ubuntu:14.04
COPY ./install_prereqs /tmp/install_prereqs
RUN yes | /tmp/install_prereqs \
  && rm -rf /var/lib/apt/lists/* \
  && rm -f /tmp/install_prereqs
ENV PATH /opt/cmake/bin:$PATH
