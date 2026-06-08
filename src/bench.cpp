// Copyright 2026 Ludicon LLC. All Rights Reserved.
//
// bench: cross-implementation performance harness.
// Loads images once, runs N iterations per (image, impl), reports median + p10/p90.

#include <stdio.h>

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    // @@ Phase 3: --iters, --threads, --impl, --blur; load once, time core call only.
    fprintf(stderr, "bench: not implemented yet (Phase 3)\n");
    return 0;
}
