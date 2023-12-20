#!/bin/bash
set -exo pipefail

export PATH="/usr/lib/ccache:$PATH"
export CCACHE_DIR="$(pwd)/ccache"
export CTEST_OUTPUT_ON_FAILURE=1

apt-get update

DEBIAN_FRONTEND=noninteractive apt-get install --no-install-{recommends,suggests} -y \
  bison ccache cmake flex g++ lib{boost-all,ssl}-dev ninja-build tzdata

mkdir build
cd build

cmake \
  -GNinja \
  -DCMAKE_BUILD_TYPE=Release \
  -DICINGA2_UNITY_BUILD=ON \
  -DUSE_SYSTEMD=OFF \
  -DICINGA2_WITH_{MYSQL,PGSQL,COMPAT,LIVESTATUS,PERFDATA,ICINGADB}=OFF \
  -DICINGA2_USER=$(id -un) \
  -DICINGA2_GROUP=$(id -gn) \
  ..

ninja
ninja test
ninja install

icinga2 daemon -C
