// Copyright 2026 Ludicon LLC. All Rights Reserved.
#pragma once

#include "ic_shared.h"

// Shared helpers for the compare/bench tools: PNG load, in-memory JPEG
// distortion, and globbing data/*.png. The stb_image/stb_image_write
// implementations live in harness.cpp so consumers don't pull them in.

struct PngImage {
    u8* rgba; // owned; free with harness_free_png; layout: w * h * 4 bytes RGBA8
    int w;
    int h;
};

// Load a PNG file as RGBA8. Returns true on success.
bool harness_load_png(const char* path, PngImage* out);
void harness_free_png(PngImage* img);

// JPEG roundtrip distortion: stbi_write_jpg → stbi_load_from_memory.
// Returns a fresh RGBA8 buffer the size of (w * h * 4), or nullptr on failure.
// Free with harness_free_distortion.
u8* harness_jpeg_distort(const u8* rgba, int w, int h, int quality);
void harness_free_distortion(u8* p);

// Iterate every *.png in data_dir. Returns the number of paths visited,
// or -1 if the glob itself failed.
typedef void (*HarnessPathCallback)(const char* path, void* ctx);
int harness_for_each_png(const char* data_dir, HarnessPathCallback cb, void* ctx);

// Basename helper (strrchr('/') + 1, or full path if no slash).
const char* harness_basename(const char* path);
