#!/bin/bash

# This script test libbot2's ubuntu package

set -euxo pipefail

pushd /tmp
# installs packages
apt-get update -y
apt-get install gdebi -y
gdebi -n lcm_1.3.95-1_linux-x86_64.deb
gdebi -n libbot2_0.0.1.$(date -u +%Y%m%d)-1_amd64.deb
# Check that files are installed
executables=(bot-wavefront-viewer bot-spy bot-rwx-viewer bot-procman-sheriff bot-procman-deputy bot-ppmsgz bot-param-tool bot-param-server bot-param-dump bot-log2mat bot-lcm-who bot-lcm-tunnel bot-lcm-logsplice bot-lcm-logfilter bot-lcmgl-viewer)

# Find missing dependency. We could look for "not found", but grepping
# "found" is easier and sufficient.
missing_libraries() {
    if [[ $(ldd $1 | grep -c found) != 0 ]]
    then
      echo "Missing libraries for $1:"
      ldd $1 | grep found
      exit 1
    fi
}

for ex in ${executables[@]}
do
  if [[ -z $ex ]]
  then
    echo "$ex not found."
  else
    # We cannot simply run each executable as some require inputs that we
    # don't have, so we simply check that they have all their dependencies
    # fulfilled.
    if [[ $(file $(which $ex) |grep -c "ELF 64-bit LSB executable, x86-64") == 1 ]]
    then
      # Test that executables find all their dependencies
      missing_libraries $(which $ex)
    # else skip for now
    fi
  fi
done
# Test library dependencies
for i in /usr/local/lib/libbot2-*
do
  missing_libraries $i
done
# Test python modules
export PYTHONPATH=${PYTHONPATH}:/opt/libbot2/0.0.1.$(date -u +%Y%m%d)/lib/python2.7/dist-packages
python_modules=(lcm bot_procman bot_param bot_log2mat bot_lcmgl bot_frames bot_core)
for i in ${python_modules[@]}
do
  python -c "$(echo "import $i")"
done
