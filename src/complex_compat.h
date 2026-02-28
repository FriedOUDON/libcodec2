#ifndef CODEC2_COMPLEX_COMPAT_H
#define CODEC2_COMPLEX_COMPAT_H

#include <math.h>
#include <complex.h>

/*
 * CMPLX/CMPLXF were standardized in C11 and may be missing in gnu99 mode.
 * Use compiler builtins when available to keep constant-initializer support.
 */
#ifndef CMPLXF
#if defined(__clang__) || defined(__GNUC__)
#define CMPLXF(real, imag) __builtin_complex((float)(real), (float)(imag))
#else
#define CMPLXF(real, imag) ((float complex)((float)(real) + (float)(imag) * I))
#endif
#endif

#ifndef CMPLX
#if defined(__clang__) || defined(__GNUC__)
#define CMPLX(real, imag) __builtin_complex((double)(real), (double)(imag))
#else
#define CMPLX(real, imag) ((double complex)((double)(real) + (double)(imag) * I))
#endif
#endif

#ifndef CMPLXL
#if defined(__clang__) || defined(__GNUC__)
#define CMPLXL(real, imag) __builtin_complex((long double)(real), (long double)(imag))
#else
#define CMPLXL(real, imag) ((long double complex)((long double)(real) + (long double)(imag) * I))
#endif
#endif

/*
 * Some Android NDK/Bionic header combinations don't expose C99 complex
 * helper prototypes/macros consistently. Fallback to compiler builtins.
 */
#ifndef crealf
#define crealf(z) ((float)(__real__(z)))
#endif

#ifndef cimagf
#define cimagf(z) ((float)(__imag__(z)))
#endif

#ifndef conjf
#define conjf(z) (CMPLXF(crealf(z), -cimagf(z)))
#endif

#ifndef cabsf
#define cabsf(z) (sqrtf(crealf(z) * crealf(z) + cimagf(z) * cimagf(z)))
#endif

#ifndef cargf
#define cargf(z) (atan2f(cimagf(z), crealf(z)))
#endif

#endif
