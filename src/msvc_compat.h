#ifndef CODEC2_MSVC_COMPAT_H
#define CODEC2_MSVC_COMPAT_H

#if defined(_MSC_VER)
/* Request math constants from the UCRT headers. */
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <complex.h>

/* MSVC stack allocator name. */
#include <malloc.h>
#ifndef alloca
#define alloca _alloca
#endif

/* Fallback in case M_PI is still unavailable. */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
 * clang-cl with UCRT headers can mix two incompatible complex models:
 * - UCRT complex APIs based on _Fcomplex structs
 * - C99 _Complex float used by this codebase
 *
 * Force C99 operators/functions when building with clang-cl.
 */
#if defined(__clang__)
#ifdef complex
#undef complex
#endif
#define complex _Complex

#ifdef I
#undef I
#endif
#define I (1.0fi)

#ifndef CMPLXF
#define CMPLXF(real, imag) __builtin_complex((float)(real), (float)(imag))
#endif
#ifndef CMPLX
#define CMPLX(real, imag) __builtin_complex((double)(real), (double)(imag))
#endif
#ifndef CMPLXL
#define CMPLXL(real, imag) __builtin_complex((long double)(real), (long double)(imag))
#endif

#ifdef crealf
#undef crealf
#endif
#ifdef cimagf
#undef cimagf
#endif
#ifdef conjf
#undef conjf
#endif
#ifdef cabsf
#undef cabsf
#endif
#ifdef cargf
#undef cargf
#endif

#define crealf(z) ((float)(__real__(z)))
#define cimagf(z) ((float)(__imag__(z)))
#define conjf(z) (__builtin_conjf(z))
#define cabsf(z) (__builtin_cabsf(z))
#define cargf(z) (__builtin_cargf(z))
#endif
#endif

#endif
