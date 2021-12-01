#ifndef __VEC_H__
#define __VEC_H__

#include <ostream>

struct vec2 {
    float x = 0.0f;
    float y = 0.0f;
    vec2& operator+=(const vec2& other);
    vec2& operator*=(float s);
    friend vec2 operator+(vec2 l, const vec2& r);
    friend vec2 operator*(vec2 l, float s);
};
std::ostream& operator<<(std::ostream& os, const vec2& v);

vec2&
vec2::operator+=(const vec2& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

vec2&
vec2::operator*=(float scale)
{
    x *= scale;
    return *this;
}

vec2
operator+(vec2 l, const vec2& r)
{
    l += r;
    return l;
}

vec2
operator*(vec2 l, float s)
{
    l *= s;
    return l; 
}

std::ostream&
operator<<(std::ostream& os, const vec2& v)
{
    os << "(" << v.x << "," << v.y << ")";
    return os;
}

#endif // __VEC_H__
