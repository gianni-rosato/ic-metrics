// Copyright 2026 Ludicon LLC. All Rights Reserved.
//
// Wrapper around the cloudinary reference impl. Their public API takes
// jxl::ImageBundle, so we round-trip our RGBA8 buffer through an in-memory
// PNG (lossless) and feed it through libjxl's SetFromBytes loader — the
// same path their CLI driver uses.

#include "impls.h"

#include "ssimulacra2.h"  // cloudinary's, in extern/cloudinary_ssimulacra2/src/

#include "lib/extras/codec.h"
#include "lib/extras/dec/color_hints.h"
#include "lib/jxl/base/span.h"
#include "lib/jxl/codec_in_out.h"

#include <stdio.h>
#include <string.h>
#include <vector>

// Encode RGBA8 as raw PPM (P6) — a libjxl-native lossless format that's just
// a tiny ASCII header plus raw RGB bytes. Replaces a stb_image_write PNG
// roundtrip that was costing ~300ms per call on a 1K image.
static void rgba_to_ppm(int w, int h, const unsigned char* rgba, std::vector<unsigned char>* out) {
    char hdr[64];
    int hlen = snprintf(hdr, sizeof(hdr), "P6\n%d %d\n255\n", w, h);
    size_t body = (size_t)w * (size_t)h * 3;
    out->resize((size_t)hlen + body);
    memcpy(out->data(), hdr, (size_t)hlen);
    unsigned char* p = out->data() + hlen;
    size_t n = (size_t)w * (size_t)h;
    for (size_t i = 0; i < n; i++) {
        p[3*i + 0] = rgba[4*i + 0];
        p[3*i + 1] = rgba[4*i + 1];
        p[3*i + 2] = rgba[4*i + 2];
    }
}

double cloudinary_compute_score(int w, int h, const unsigned char* orig, const unsigned char* dist) {
    std::vector<unsigned char> orig_ppm, dist_ppm;
    rgba_to_ppm(w, h, orig, &orig_ppm);
    rgba_to_ppm(w, h, dist, &dist_ppm);

    jxl::CodecInOut io1, io2;
    auto orig_span = jxl::Span<const uint8_t>(orig_ppm.data(), orig_ppm.size());
    auto dist_span = jxl::Span<const uint8_t>(dist_ppm.data(), dist_ppm.size());
    if (!jxl::SetFromBytes(orig_span, jxl::extras::ColorHints(), &io1)) {
        fprintf(stderr, "cloudinary: SetFromBytes failed (orig)\n");
        return 0.0;
    }
    if (!jxl::SetFromBytes(dist_span, jxl::extras::ColorHints(), &io2)) {
        fprintf(stderr, "cloudinary: SetFromBytes failed (dist)\n");
        return 0.0;
    }

    // PPM has no alpha, so HasAlpha() is always false here — input alpha is
    // dropped (which matches ssimulacra2's spec for our RGBA8-sRGB inputs).
    return ComputeSSIMULACRA2(io1.Main(), io2.Main()).Score();
}
