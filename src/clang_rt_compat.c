#include <complex.h>

#if defined(_MSC_VER) && defined(__clang__)
/*
 * clang-cl can emit compiler-rt helper calls for complex multiplication.
 * Provide a local implementation so linking does not depend on external
 * compiler-rt libraries on GitHub Actions Windows images.
 */
float _Complex __mulsc3(float a, float b, float c, float d) {
    return __builtin_complex((a * c) - (b * d), (a * d) + (b * c));
}
#endif

