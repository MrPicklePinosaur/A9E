#ifndef __VEC_H__
#define __VEC_H__

#include <ostream>
#include <cmath>

struct vec2 {
    float x = 0.0f;
    float y = 0.0f;

    float magnitude();
    float dot(const vec2& other);
    vec2 normalize();
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
};
std::ostream& operator<<(std::ostream& os, const vec2& v);

// try to write this without sqrt
float
vec2::magnitude()
{
    return sqrt(x*x+y*y);
}

float
vec2::dot(const vec2& other)
{
    return x*other.x+y*other.y;
}

vec2
vec2::normalize()
{
    return vec2::normalize(*this);
}

vec2&
vec2::operator+=(const vec2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

vec2&
vec2::operator-=(const vec2& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

vec2&
vec2::operator*=(float s)
{
    x *= s;
    return *this;
}

vec2&
vec2::operator/=(float s)
{
    x /= s;
    return *this;
}

vec2
operator+(vec2 l, const vec2& r)
{
    l += r;
    return l;
}

vec2
operator-(vec2 l, const vec2& r)
{
    l -= r;
    return l;
}

vec2
operator*(vec2 l, float s)
{
    l *= s;
    return l; 
}

vec2
operator/(vec2 l, float s)
{
    l /= s;
    return l;
}

std::ostream&
operator<<(std::ostream& os, const vec2& v)
{
    os << "(" << v.x << "," << v.y << ")";
    return os;
}

vec2
vec2::zero()
{
    return vec2{};
}

vec2
vec2::normalize(vec2 v)
{
    float mag = v.magnitude();
    if (mag == 0) return vec2::zero();
    return v/mag;
}
#endif // __VEC_H__
