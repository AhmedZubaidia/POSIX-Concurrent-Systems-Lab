#!/bin/sh
# ----------------------------------------------------------------------------
# One-shot launcher: build every binary via the makefile, then start the
# orchestrator. The orchestrator (`main`) execs the other binaries by
# relative path, so they must sit alongside it in the repo root.
# ----------------------------------------------------------------------------

set -eu

make
./main
