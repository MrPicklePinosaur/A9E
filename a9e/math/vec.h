#ifndef __A9E_MATH_VEC_H__
#define __A9E_MATH_VEC_H__

#include <ostream>
#include <cmath>

struct vec2 {
    float x = 0.0f;
    float y = 0.0f;

    float magnitude() const;
    float dot(const vec2& other) const;
    vec2 normalize() const;
    vec2& operator+=(const vec2& other);
    vec2& operator-=(const vec2& other);
    vec2& operator*=(float s);
    vec2& operator/=(float s);
    friend vec2 operator+(vec2 l, const vec2& r);
    friend vec2 operator-(vec2 l, const vec2& r);
    friend vec2 operator*(vec2 l, float s);
    friend vec2 operator/(vec2 l, float s);

    static vec2 zero();
    static vec2 normalize(vec2 v);
    static vec2 projection(const vec2& v1, const vec2& v2);
};
std::ostream& operator<<(std::ostream& os, const vec2& v);

#endif // __A9E_MATH_VEC_H__
