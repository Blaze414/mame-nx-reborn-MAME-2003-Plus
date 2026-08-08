#!/bin/sh
# Reproducible NRO build using devkitPro's official devkitA64 image.
# Run from repo root: ./docker-build.sh          (incremental)
#                     ./docker-build.sh clean    (full rebuild)
#
# Outputs release/mame.nro, release/mame.nso, release/mame.pfs0
set -e

if [ "$1" = "clean" ]; then
  PRE='make clean'
else
  # The archive pattern rule is timestamp-based, so it will happily keep an old
  # .a whose member list no longer matches the .mak files. Always rebuild them.
  PRE='rm -f obj/mame/*.a'
fi

docker run --rm -v "$(pwd)":/src -w /src devkitpro/devkita64 sh -c "
  $PRE
  make -j\$(nproc)
"
