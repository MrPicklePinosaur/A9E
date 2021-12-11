#ifndef __MATH_VEC_H__
#define __MATH_VEC_H__

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

// try to write this without sqrt
float
vec2::magnitude() const
{
    return sqrt(x*x+y*y);
}

float
vec2::dot(const vec2& other) const
{
    return x*other.x+y*other.y;
}

vec2
vec2::normalize() const
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
    y *= s;
    return *this;
}

vec2&
vec2::operator/=(float s)
{
    x /= s;
    y /= s;
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

vec2
vec2::projection(const vec2& v1, const vec2& v2)
{
    vec2 v2_unit = v2.normalize();
    return v2_unit*v1.dot(v2_unit);
}

#endif // __MATH_VEC_H__
