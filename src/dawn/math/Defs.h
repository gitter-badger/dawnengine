/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2016 (git@davedissian.com)
 */
#pragma once

#include <cmath>
#ifdef M_PI
#undef M_PI
#endif

namespace dw {

static const float M_PI = 3.14159265358979323846264338327950288f;
static const float M_HALF_PI = M_PI * 0.5f;
static const float M_EPSILON = std::numeric_limits<float>::epsilon();
static const float M_LARGE_EPSILON = 0.00005f;
static const float M_LARGE_VALUE = 100000000.0f;
static const float M_INFINITY = HUGE_VALF;
static const float M_DEGTORAD = M_PI / 180.0f;
static const float M_DEGTORAD_OVER_2 = M_PI / 360.0f;
static const float M_RADTODEG = 1.0f / M_DEGTORAD;

// Vectors
using Vec2 = math::float2;
using Vec3 = math::float3;
using Vec4 = math::float4;

// Matrices
using Mat22 = math::float2x2;
using Mat23 = math::float2x3;
using Mat24 = math::float2x3;
using Mat33 = math::float3x3;
using Mat34 = math::float3x4;
using Mat44 = math::float4x4;
using Mat2 = Mat22;
using Mat3 = Mat33;
using Mat4 = Mat44;

// Quaternion
using Quat = math::Quat;

// Plane
using Plane = math::Plane;

//// Floating point functions

/// Fast inverse square root. Originally from Quake III.
inline float fastSqrt(float value) {
    float half = 0.5f * value;
    int i = *(int*)&value;

    // Initial guess y0
    i = 0x5f3759df - (i >> 1);

    // Perform a single newton iteration
    value = *(float*)&i;
    value = value * (1.5f - half * value * value);
    return value;
}

/// Linear interpolation between two float values.
inline float lerp(float lhs, float rhs, float t) {
    return lhs * (1.0f - t) + rhs * t;
}

/// Linear interpolation between two double values.
inline double lerp(double lhs, double rhs, float t) {
    return lhs * (1.0f - t) + rhs * t;
}

/// Linear interpolation between two float values taking into account the rate of time.
/// NOTE: in 1 second, value will be t% between the lhs and rhs.
inline float lerp(float lhs, float rhs, float t, float dt) {
    return lerp(lhs, rhs, 1.0f - pow(1.0f - t, dt));
}

/// Linear interpolation between two quaternion values taking into account the rate of time.
inline Quat lerp(const Quat& a, const Quat& b, float t, float dt) {
    return Quat::Slerp(a, b, 1.0f - pow(1.0f - t, dt));
}

/// Returns the greatest integer which is less than the input value
inline float floor(float value) {
    return ::floorf(value);
}

/// Returns the smallest integer which is greater than the input value
inline float ceil(float value) {
    return ::ceilf(value);
}

/// Return the sign of a float (-1, 0 or 1.)
inline float sign(float value) {
    return value > 0.0f ? 1.0f : (value < 0.0f ? -1.0f : 0.0f);
}

/// Fuzzy equals operator for floats.
inline bool floatEq(float a, float b, float epsilon = M_EPSILON) {
    return ::fabs(a - b) <= epsilon;
}

/// Fuzzy equals operator for doubles.
inline bool floatEq(double a, double b, double epsilon = M_EPSILON) {
    return ::fabs(a - b) <= epsilon;
}

/// Clamp a float to a range.
inline float clamp(float value, float min, float max) {
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

/// Smoothly damp between values.
inline float smoothStep(float lhs, float rhs, float t) {
    t = clamp((t - lhs) / (rhs - lhs), 0.0f, 1.0f);  // Saturate t
    return t * t * (3.0f - 2.0f * t);
}

/// Floating-point modulo
/// Note: The result (the remainder) has same sign as the divisor. Similar to matlab's mod(),
///	      rather than cmath's fmod(). Eg: mod(-3,4) = 1, fmod(-3,4) = -3
inline float mod(float x, float y) {
    if (floatEq(y, 0.0f))
        return x;

    float m = x - y * floor(x / y);
    if (y > 0)  // modulo range: [0..y)
    {
        if (m >= y)  // mod(-1e-16, 360.0): m = 360.0
            return 0;

        if (m < 0) {
            if (floatEq(y + m, y))
                return 0;
            else
                return y + m;  // mod(106.81415022205296, _TWO_PI ): m = -1.421e-14
        }
    } else  // modulo range: (y..0]
    {
        if (m <= y)  // mod(1e-16, -360.0): m = -360.0
            return 0;

        if (m > 0) {
            if (floatEq(y + m, y))
                return 0;
            else
                return y + m;  // mod(-106.81415022205296, -_TWO_PI): m = 1.421e-14
        }
    }

    return m;
}

//// Integer functions

/// Clamp an integer to a range.
inline int clamp(int value, int min, int max) {
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}

/// Check whether an unsigned integer is a power of two.
inline bool isPow2(unsigned value) {
    if (!value)
        return true;
    while (!(value & 1))
        value >>= 1;
    return value == 1;
}

/// Round up to next power of two.
inline unsigned nextPow2(unsigned value) {
    unsigned ret = 1;
    while (ret < value && ret < 0x80000000)
        ret <<= 1;
    return ret;
}

/// Count the number of set bits in a mask.
inline unsigned countSet(unsigned value) {
    // Brian Kernighan's method
    unsigned count = 0;
    for (count = 0; value; count++)
        value &= value - 1;
    return count;
}

/// Update a hash with the given 8-bit value using the SDBM algorithm.
inline unsigned sdbmHash(unsigned hash, unsigned char c) {
    return c + (hash << 6) + (hash << 16) - hash;
}

//// Templated functions

/// Limits a value to a certain range by wrapping it past the constraints.
template <class T> T wrap(const T& value, const T& min, const T& max) {
    return mod(value - min, max - min) + min;
}

/// Takes one step from a value to a target with a given step. If the distance is less then the
/// step value, then it just returns the target value.
template <class T> T step(const T& value, const T& step, const T& target) {
    if (target > value)
        return min(target, value + step);
    if (target < value)
        return max(target, value - step);
    return value;
}

/// Units

enum DistUnits { UNIT_M, UNIT_KM, UNIT_MI, UNIT_AU, UNIT_LY, UNIT_PC };

enum MassUnits {
    UNIT_G,
    UNIT_KG,
    UNIT_T,
    UNIT_SM  // Solar Mass
};

enum TimeUnits {
    UNIT_SECOND,
    UNIT_MINUTE,
    UNIT_HOUR,
    UNIT_DAY,
    UNIT_YEAR,
};

template <class T> T convUnit(T input, DistUnits outUnit, DistUnits inUnit = UNIT_M) {
    // Unit to value lookup function
    auto lookup = [](DistUnits unit) -> T {
        switch (unit) {
            case UNIT_M:
                return (T)1.0;

            case UNIT_KM:
                return (T)1e3;  // 1 km = 10^3 m

            case UNIT_MI:
                return (T)1609.344;  // 1 mile = 1609.344 m

            case UNIT_AU:
                return (T)149597870700.0;  // 1 astronomical unit = 1.5x10^11 m

            case UNIT_LY:
                return (T)9.46073047e15;  // 1 light-year = 9.46x10^15 m

            case UNIT_PC:
                return (T)3.08567758e16;  // 1 parsec = 3.057x10^16 m

            default:
                assert(false);
                return (T)0.0;
        }
    };

    // Special cases: light-years to parsecs; parsecs to light-years.
    if (inUnit == UNIT_LY && outUnit == UNIT_PC) {
        return input * (T)0.306594845;  // 1 parsec = 0.307 light-years
    }

    if (inUnit == UNIT_PC && outUnit == UNIT_LY) {
        return input * (T)3.26163344332;  // 3.26 light years = 1 parsec
    }

    // Lookup the units and convert
    return (input / lookup(inUnit)) * lookup(outUnit);
}

template <class T> T convUnit(T input, MassUnits outUnit, MassUnits inUnit = UNIT_KG) {
    // Unit to value lookup function
    auto lookup = [](MassUnits unit) -> T {
        switch (unit) {
            case UNIT_G:
                return (T)0.001;  // 1 g = 0.001 kg

            case UNIT_KG:
                return (T)1.0;

            case UNIT_T:
                return (T)1000.0;  // 1 tonne = 1000 kg

            case UNIT_SM:
                return (T)1.9891e30;  // 1 solar mass = 1.99x10^30 kg

            default:
                assert(false);
                return (T)0.0;
        }
    };

    // Lookup the units and convert
    return (input / lookup(inUnit)) * lookup(outUnit);
}
}
