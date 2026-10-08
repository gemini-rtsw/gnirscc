#!/bin/sh
# Runs inside the CI build container BEFORE `dnf builddep` (hook provided by
# gemini-rtsw-ci/build_rpm.sh).
#
# The 32-bit 68k cross-tools (gem-vxworks52-linux) need glibc.i686, which must
# exactly match the installed x86_64 glibc. The rockylinux base image can lag
# the mirrors, making the builddep transaction unresolvable. Upgrading glibc
# first keeps the two arches in lockstep -- the same fix as gmoscc.
set -e
dnf -y upgrade glibc
