#!/bin/bash
set -exo pipefail

mkdir build
cd build

PATH="/usr/lib/ccache:$PATH" cmake \
  -GNinja \
  -DCMAKE_BUILD_TYPE=Release \
  -DICINGA2_UNITY_BUILD=ON \
  -DUSE_SYSTEMD=OFF \
  -DICINGA2_WITH_{MYSQL,PGSQL,COMPAT,LIVESTATUS,PERFDATA,ICINGADB}=OFF \
  -DICINGA2_USER=$(id -un) \
  -DICINGA2_GROUP=$(id -gn) \
  ..

CCACHE_DIR="$(pwd)/ccache" ninja
CTEST_OUTPUT_ON_FAILURE=1 ninja test
ninja install

icinga2 daemon -C
