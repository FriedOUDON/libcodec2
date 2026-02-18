#pragma once

#ifdef __ANDROID__

#include <math.h>

typedef struct {
    float real;
    float imag;
} _android_complex_float;

#define crealf(x) ((x).real)
#define cimagf(x) ((x).imag)
#define conjf(x) ((_android_complex_float){(x).real, -(x).imag})
#define cabsf(x) sqrtf((x).real*(x).real + (x).imag*(x).imag)
#define cargf(x) atan2f((x).imag, (x).real)

#endif
