// Copyright 2026 Ludicon LLC. All Rights Reserved.
//
// compare: cross-implementation correctness harness.
// Runs each ssimulacra2 implementation over every image in data/,
// generating on-the-fly distortions and writing scores as CSV.

#include <stdio.h>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    // @@ Phase 1: glob data/*.png, generate JPEG distortions, run each impl, emit CSV.
    fprintf(stderr, "compare: not implemented yet (Phase 1)\n");
    return 0;
}
