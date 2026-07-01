// Copyright 2026 Ludicon LLC. All Rights Reserved.
// Portions copyright (c) Jon Sneyers, Cloudinary. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

#include <stddef.h> // size_t

// Computes the SSIMULACRA 2 score between reference image 'orig' and
// distorted image 'dist'. Both buffers are RGBA8; alpha is currently
// honored per the ssimu2_alpha_blend var.
//
// Minimum supported size is 8x8 (matches the cloudinary and rust-av
// references). SSIMULACRA2 is multi-scale and no scale runs below 8px in
// either dimension, leaving the score undefined; such inputs return NaN
// (and trip an assert in debug builds) rather than a bogus perfect 100.
//
// Caller-managed scratch buffer. Allocate ic_*_score_scratch_size(w, h) bytes
// (malloc is fine — any allocator returning a float-aligned pointer works)
// and pass via scratch_ptr. Reuse across calls with the same w/h to avoid
// per-call allocations.
size_t ic_ssimulacra2_score_scratch_size(int w, int h);
double ic_ssimulacra2_score(int w, int h, const unsigned char *orig, const unsigned char *dist, void *scratch_ptr,
                            unsigned char *error_map = nullptr, bool alpha_blend = false, float background = 0.5f);

// Single-scale SSIM: defined at any size (down to 1x1), so there is no
// minimum-size restriction. On very small images the Gaussian just becomes
// edge-clamp dominated, but the score stays meaningful.
size_t ic_ssim_score_scratch_size(int w, int h);
double ic_ssim_score(int w, int h, const unsigned char *orig, const unsigned char *dist, void *scratch_ptr,
                     unsigned char *error_map = nullptr);

// MS-SSIM (Multi-Scale SSIM) — Wang/Simoncelli/Bovik 2003. Same RGBA8 input
// and scratch-buffer pattern as ic_ssim_score. Score is in [0, 1] (1 means
// identical). The optional error_map is filled at the finest scale.
//
// Multi-scale, so like SSIMULACRA2 the minimum supported size is 8x8; smaller
// inputs return NaN (and assert in debug) instead of a meaningless 0.
size_t ic_msssim_score_scratch_size(int w, int h);
double ic_msssim_score(int w, int h, const unsigned char *orig, const unsigned char *dist, void *scratch_ptr,
                       unsigned char *error_map = nullptr);
