#!/bin/bash

# This script test libbot2's ubuntu package

set -euxo pipefail

if [ -z "$1" ]; then
  readonly timestamp=$(date -u +%Y%m%d)
else
  readonly timestamp=$1
fi

pushd /tmp
# installs packages
apt-get update -qq
apt-get install --no-install-recommends -y devscripts gdebi
gdebi -n lcm_1.4.0-2_amd64.deb
gdebi -n libbot2_0.0.1.${timestamp}-1_amd64.deb
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

export PATH=${PATH}:/opt/libbot2/0.0.1.${timestamp}/bin
for ex in ${executables[@]}
do
  if [[ -z $ex ]]
  then
    echo "$ex not found."
  else
    # We cannot simply run each executable as some require inputs that we
    # don't have, so we simply check that they have all their dependencies
    # fulfilled.
    if [[ $(file $(which $ex) | grep -c "ELF 64-bit LSB .*, x86-64") == 1 ]]
    then
      hardening-check $(which $ex) || true
      # Test that executables find all their dependencies
      missing_libraries $(which $ex)
    # else skip for now
    fi
  fi
done
# Test library dependencies
for i in /opt/libbot2/0.0.1.${timestamp}/lib/libbot2-*.so
do
  hardening-check $i || true
  missing_libraries $i
done
for i in /opt/libbot2/0.0.1.${timestamp}/lib/liblcmtypes_bot2-*.a
do
  hardening-check $i || true
done
# Test python modules
export PYTHONPATH=${PYTHONPATH:+$PYTHONPATH:}/opt/libbot2/0.0.1.${timestamp}/lib/python3/dist-packages:/opt/lcm/1.4.0/lib/python3/dist-packages
python_modules=(lcm bot_procman bot_param bot_log2mat bot_lcmgl bot_frames bot_core)
for i in ${python_modules[@]}
do
  python3 -c "$(echo "import $i")"
done
