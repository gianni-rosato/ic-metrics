// Copyright 2026 Ludicon LLC. All Rights Reserved.
//
// Wrapper around the rust_av_bridge C ABI (extern/rust_av_bridge/src/lib.rs).

#include "impls.h"

extern "C" double rust_av_score(
    const unsigned char* orig, const unsigned char* dist, unsigned int w, unsigned int h);

double rust_av_compute_score(int w, int h, const unsigned char* orig, const unsigned char* dist) {
    return rust_av_score(orig, dist, (unsigned)w, (unsigned)h);
}
