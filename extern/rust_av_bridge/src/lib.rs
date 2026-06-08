// Copyright 2026 Ludicon LLC. All Rights Reserved.
//
// C-ABI bridge over rust-av/ssimulacra2.
// Mirrors what ssimulacra2_bin's compare_images does: builds an Rgb<f32>
// from sRGB bytes and calls compute_frame_ssimulacra2.

use ssimulacra2::{compute_frame_ssimulacra2, ColorPrimaries, Rgb, TransferCharacteristic};

fn rgba8_to_rgb_f32(buf: &[u8]) -> Vec<[f32; 3]> {
    buf.chunks_exact(4)
        .map(|c| [c[0] as f32 / 255.0, c[1] as f32 / 255.0, c[2] as f32 / 255.0])
        .collect()
}

/// # Safety
/// `orig` and `dist` must each point to at least `w * h * 4` bytes of RGBA8 data.
#[no_mangle]
pub unsafe extern "C" fn rust_av_score(
    orig: *const u8,
    dist: *const u8,
    w: u32,
    h: u32,
) -> f64 {
    let pixel_count = (w as usize) * (h as usize);
    let orig_slice = std::slice::from_raw_parts(orig, pixel_count * 4);
    let dist_slice = std::slice::from_raw_parts(dist, pixel_count * 4);

    let orig_data = rgba8_to_rgb_f32(orig_slice);
    let dist_data = rgba8_to_rgb_f32(dist_slice);

    let src = match Rgb::new(
        orig_data,
        w as usize,
        h as usize,
        TransferCharacteristic::SRGB,
        ColorPrimaries::BT709,
    ) {
        Ok(r) => r,
        Err(_) => return 0.0,
    };

    let dst = match Rgb::new(
        dist_data,
        w as usize,
        h as usize,
        TransferCharacteristic::SRGB,
        ColorPrimaries::BT709,
    ) {
        Ok(r) => r,
        Err(_) => return 0.0,
    };

    compute_frame_ssimulacra2(src, dst).unwrap_or(0.0)
}
