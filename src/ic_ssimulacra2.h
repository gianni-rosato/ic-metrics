// Copyright (c) Ignacio Castaño <castano@gmail.com>
// Copyright (c) Jon Sneyers, Cloudinary. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

struct MsssimScale {
  double avg_ssim[3 * 2];
  double avg_edgediff[3 * 4];
};

static const int kNumScales = 6;
struct Msssim {
  MsssimScale scales[kNumScales];

  double Score() const;
};

// Computes the SSIMULACRA 2 score between reference image 'orig' and
// distorted image 'dist'. Both of them are in RGBA8 format. Alpha is ignored.
Msssim ComputeSSIMULACRA2(int w, int h, const unsigned char * orig, const unsigned char * dist, unsigned char* error_map = nullptr);
double ComputeSSIMULACRA2Score(int w, int h, const unsigned char * orig, const unsigned char * dist, unsigned char* error_map = nullptr);

double ComputeSSIMScore(int w, int h, const unsigned char * orig, const unsigned char * dist, unsigned char* error_map = nullptr);
