#!/bin/bash

###
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
printf "== Moving to *build* directory - creating if needed ... "
mkdir -p "$DIR/build"
printf "OK\n"
cd "$DIR/build"

###
printf "== Creating build system\n"

if cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ; then
  printf "== Created build system OK\n"
else
  printf "== FAILED to created build system - Exiting\n"
  exit 1
fi

###
printf "== Autobuilding (smoke test)\n"

if cmake --build . --config Release --parallel $(nproc); then
  printf "== Autobuild was OK\n"
else
  printf "== FAILED to Autobuild\n - Exiting\n"
  exit 1
fi

###
printf "== Return to child directory ..."
cd ..
printf "OK\n"
exit 0