/* gdkcolordefs.h
 *
 * Copyright 2024 Matthias Clasen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

/* Note that this header is shared between the color state implementation
 * and tests, and must not include other headers.
 */

static inline float
srgb_oetf (float v)
{
  if (v > 0.0031308f)
    return 1.055f * powf (v, 1.f / 2.4f) - 0.055f;
  else
    return 12.92f * v;
}

static inline float
srgb_eotf (float v)
{
  if (v >= 0.04045f)
    return powf (((v + 0.055f) / (1.f + 0.055f)), 2.4f);
  else
    return v / 12.92f;
}

static inline float
gamma22_oetf (float v)
{
  return powf (v, 1.f / 2.2f);
}

static inline float
gamma22_eotf (float v)
{
  return powf (v, 2.2f);
}

static inline float
gamma28_oetf (float v)
{
  return powf (v, 1.f / 2.8f);
}

static inline float
gamma28_eotf (float v)
{
  return powf (v, 2.8f);
}

static inline float
pq_eotf (float v)
{
  float ninv = (1 << 14) / 2610.0;
  float minv = (1 << 5) / 2523.0;
  float c1 = 3424.0 / (1 << 12);
  float c2 = 2413.0 / (1 << 7);
  float c3 = 2392.0 / (1 << 7);

  float x = powf (MAX ((powf (v, minv) - c1), 0) / (c2 - (c3 * (powf (v, minv)))), ninv);

  return x * 10000 / 203.0;
}

static inline float
pq_oetf (float v)
{
  float x = v * 203.0 / 10000.0;
  float n = 2610.0 / (1 << 14);
  float m = 2523.0 / (1 << 5);
  float c1 = 3424.0 / (1 << 12);
  float c2 = 2413.0 / (1 << 7);
  float c3 = 2392.0 / (1 << 7);

  return powf (((c1 + (c2 * powf (x, n))) / (1 + (c3 * powf (x, n)))), m);
}

static inline float
bt709_eotf (float v)
{
  const float a = 1.099;
  const float d = 0.0812;

  if (v < d)
    return  v / 4.5f;
  else
    return powf ((v + (a - 1)) / a, 1 / 0.45f);
}

static inline float
bt709_oetf (float v)
{
  const float a = 1.099;
  const float b = 0.018;

  if (v < b)
    return v * 4.5f;
  else
    return a * powf (v, 0.45f) - (a - 1);
}

static inline float
hlg_eotf (float v)
{
  const float a = 0.17883277;
  const float b = 0.28466892;
  const float c = 0.55991073;

  if (v <= 0.5)
    return (v * v) / 3;
  else
    return (expf ((v - c) / a) + b) / 12.0;
}

static inline float
hlg_oetf (float v)
{
  const float a = 0.17883277;
  const float b = 0.28466892;
  const float c = 0.55991073;

  if (v <= 1/12.0)
    return sqrtf (3 * v);
  else
    return a * logf (12 * v - b) + c;
}

/* See http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
 * for how to derive the abc_to_xyz matrices from chromaticity coordinates.
 */

static const float identity[9] = {
  1, 0, 0,
  0, 1, 0,
  0, 0, 1,
};

static const float srgb_to_xyz[9] = {
  0.4124564,  0.3575761,  0.1804375,
  0.2126729,  0.7151522,  0.0721750,
  0.0193339,  0.1191920,  0.9503041,
};

static const float xyz_to_srgb[9] = {
  3.2404542, -1.5371385, -0.4985314,
 -0.9692660,  1.8760108,  0.0415560,
  0.0556434, -0.2040259,  1.0572252,
};

static const float rec2020_to_xyz[9] = {
  0.6369580,  0.1446169,  0.1688810,
  0.2627002,  0.6779981,  0.0593017,
  0.0000000,  0.0280727,  1.0609851,
};

static const float xyz_to_rec2020[9] = {
  1.7166512, -0.3556708, -0.2533663,
 -0.6666844,  1.6164812,  0.0157685,
  0.0176399, -0.0427706,  0.9421031,
};

static const float pal_to_xyz[9] = {
  0.4305538,  0.3415498,  0.1783523,
  0.2220043,  0.7066548,  0.0713409,
  0.0201822,  0.1295534,  0.9393222,
};

static const float xyz_to_pal[9] = {
  3.0633611, -1.3933902, -0.4758237,
 -0.9692436,  1.8759675,  0.0415551,
  0.0678610, -0.2287993,  1.0690896,
};

static const float ntsc_to_xyz[9] = {
  0.3935209,  0.3652581,  0.1916769,
  0.2123764,  0.7010599,  0.0865638,
  0.0187391,  0.1119339,  0.9583847,
};

static const float xyz_to_ntsc[9] = {
  3.5060033, -1.7397907, -0.5440583,
 -1.0690476,  1.9777789,  0.0351714,
  0.0563066, -0.1969757,  1.0499523,
};

static const float p3_to_xyz[9] = {
  0.4865709,  0.2656677,  0.1982173,
  0.2289746,  0.6917385,  0.0792869,
  0.0000000,  0.0451134,  1.0439444,
};

static const float xyz_to_p3[9] = {
  2.4934969, -0.9313836, -0.4027108,
 -0.8294890,  1.7626641,  0.0236247,
  0.0358458, -0.0761724,  0.9568845,
};

/* premultiplied matrices for default conversions */

static const float rec2020_to_srgb[9] = {
  1.660227, -0.587548, -0.072838,
 -0.124553,  1.132926, -0.008350,
 -0.018155, -0.100603,  1.118998,
};

static const float srgb_to_rec2020[9] = {
  0.627504, 0.329275, 0.043303,
  0.069108, 0.919519, 0.011360,
  0.016394, 0.088011, 0.895380,
};

/* oklab conversion */

static float
from_oklab_nl (float v)
{
  return v * v * v;
}

static float
to_oklab_nl (float v)
{
  return cbrtf (v);
}

static const float oklab_to_lms[9] = {
  1,  0.3963377774,  0.2158037573,
  1, -0.1055613458, -0.0638541728,
  1, -0.0894841775, -1.2914855480
};

static const float lms_to_srgb[9] = {
  4.0767416621, -3.3077115913,  0.2309699292,
 -1.2684380046,  2.6097574011, -0.3413193965,
 -0.0041960863, -0.7034186147,  1.7076147010,
};

static const float srgb_to_lms[9] = {
  0.4122214708, 0.5363325363, 0.0514459929,
  0.2119034982, 0.6806995451, 0.1073969566,
  0.0883024619, 0.2817188376, 0.6299787005,
};

static const float lms_to_oklab[9] = {
  0.2104542553,  0.7936177850, -0.0040720468,
  1.9779984951, -2.4285922050,  0.4505937099,
  0.0259040371,  0.7827717662, -0.8086757660,
};

static const float rec2020_to_lms[9] = {
  0.616645, 0.360250, 0.023064,
  0.265075, 0.635874, 0.099059,
  0.100076, 0.203907, 0.696161,
};

static const float lms_to_rec2020[9] = {
  2.140325, -1.246734, 0.106491,
  -0.884665, 2.163141, -0.278489,
  -0.048559, -0.454366, 1.502711,
};
