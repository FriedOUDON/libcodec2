#ifndef CODEC2_MSVC_COMPAT_H
#define CODEC2_MSVC_COMPAT_H

#if defined(_MSC_VER)
/* Request math constants from the UCRT headers. */
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

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
 * UCRT can define "complex" as "_complex" in C mode, which breaks C99
 * complex declarations in this codebase. Restore C99 meaning.
 */
#ifdef complex
#undef complex
#endif
#define complex _Complex
#endif

#endif
