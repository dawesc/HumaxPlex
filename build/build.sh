#!/bin/bash
cd $(dirname $0)
cd ..
set -e


for arg in "$@"; do
  case $arg in
    clean)   CLEAN=1 ;;
    package) PACKAGE=1 ;;
    verbose) VERBOSE=1 ;;

    debug)   BUILD_TYPE=Debug ;;
    release) BUILD_TYPE=Release ;;

    sanitize) SANITIZE=1 ;;

    gmake) MAKETOOL=gmake ;;
    ninja) MAKETOOL=ninja ;;

    clang)   CXX=clang++ ;;
    clang38) CXX=clang++-3.8 ;;
    gcc)     CXX=g++ ;;
    gcc6)    CXX=g++-6 ;;
    gcc5)    CXX=g++-5 ;;
    gcc49)   CXX=g++-4.9 ;;
    gcc48)   CXX=g++-4.8 ;;

    scan)   SCAN=scan-build ;;
    scan38) SCAN=scan-build-3.8 ;;
  esac
done

PREFIX="/tmp/humax_plex"

if [ -n "$MSYSTEM" ]; then
  # Windows (MSYS2)
  if [ "$MSYSTEM" = "MINGW32" ]; then
    [ -z "$PREFIX" ] && PREFIX="/c/eftlab/win32"
  elif [ "$MSYSTEM" = "MINGW64" ]; then
    [ -z "$PREFIX" ] && PREFIX="/c/eftlab/win64"
  else
    echo "Unsupported MSYSTEM: $MSYSTEM"
    exit 1
  fi
  [ -z "$NCORES" ] && NCORES=$(($NUMBER_OF_PROCESSORS - 1))
  [ -z "$MAKETOOL" ] && MAKETOOL=gmake
elif [ -e /proc/cpuinfo ]; then
  # Linux
  [ -z "$PREFIX" ] && PREFIX="/usr/local/bp"
  [ -z "$NCORES" ] && NCORES=$(($(grep -Pc '^processor\s*:\s*\d+$' /proc/cpuinfo) - 1))
  [ -z "$MAKETOOL" ] && MAKETOOL=ninja
else
  # OSX
  [ -z "$PREFIX" ] && PREFIX="/usr/local/bp"
  [ -z "$NCORES" ] && NCORES=$(($(sysctl hw.ncpu | awk '{print $2}') - 1))
  [ -z "$MAKETOOL" ] && MAKETOOL=ninja
fi

#single-core fix
if [ "$NCORES" == 0 ]; then NCORES=1; fi

if [ "$MAKETOOL" = ninja ] && which ninja &>/dev/null; then
  CMAKE="cmake -G 'Ninja'"
  MAKE="ninja -j$NCORES"
  [ -n "$VERBOSE" ] && MAKE="$MAKE -v"
elif [ -n "$MSYSTEM" ]; then
  CMAKE="cmake -G 'MSYS Makefiles'"
  MAKE="make -j$NCORES"
else
  CMAKE="cmake -G 'Unix Makefiles'"
  MAKE="make -j$NCORES"
fi

if [ -n "$SANITIZE" ]; then
  CMAKE_ARGS="-DCMAKE_CXX_FLAGS='-fsanitize=leak -fsanitize=address -fsanitize=undefined'"
fi

if [ -n "$SCAN" ]; then
  export CCC_CXX=$(echo $SCAN | sed 's/scan-build/clang++/')
  CXX=$($SCAN sh -c 'echo CXX=$CXX' | grep -Po '(?<=CXX=).*')
  MAKE="$SCAN $MAKE"
fi

export CCACHE_COMPRESS=1
export VERBOSE
export CXX

SCRIPT="
  set -ex"
[ -n "$CLEAN" ] && SCRIPT="$SCRIPT
  rm -rf work"
SCRIPT="$SCRIPT
  mkdir -p work
  cd work
  $CMAKE .. \\"
[ -n "$NO_BUILD_TESTS" ] && SCRIPT="$SCRIPT
    -DBP_BUILD_TESTS=off \\"
[ -n "$BUILD_TYPE" ] && SCRIPT="$SCRIPT
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \\"
[ -n "$CXX" ] && SCRIPT="$SCRIPT
    -DCMAKE_CXX_COMPILER=$CXX \\"
SCRIPT="$SCRIPT
    -DCMAKE_INSTALL_PREFIX=$PREFIX \\
    -DCMAKE_PREFIX_PATH=$PREFIX \\
    -DCMAKE_EXPORT_COMPILE_COMMANDS=on \\
    $CMAKE_ARGS
  $MAKE
  $MAKE install"
[ -n "$PACKAGE" ] && SCRIPT="$SCRIPT
  $MAKE package"

set +e
script -V 2>&1 >/dev/null
if [ "$?" -eq "0" ]; then
  script build.log -c "time ($SCRIPT)"
else
  TMP_FILE=/tmp/build_`date +'%s'`.sh
  echo "$SCRIPT" > $TMP_FILE
  chmod u+x $TMP_FILE
  cat $TMP_FILE
  script build.log time $TMP_FILE
  rm $TMP_FILE
fi