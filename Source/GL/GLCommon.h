#pragma once

#include <gl/glew.h>
#include <initializer_list>
#include <cfloat>
#include <cmath>

template<typename Scalar>
inline Scalar sqrtx(Scalar s)
{
    return sqrt(s);
}

template<>
inline float sqrtx(float s)
{
    return sqrtf(s);
}

template<>
inline long double sqrtx(long double s)
{
    return sqrtl(s);
}

template<typename Scalar>
inline Scalar ToRadian(Scalar degree)
{
    return degree * (Scalar)3.14159265358979323846264338327950288419716939937510l / (Scalar)180.0;
}

template<typename Scalar>
inline Scalar ToDegree(Scalar radian)
{
    return radian / (Scalar)3.14159265358979323846264338327950288419716939937510l * (Scalar)180.0;
}

template<typename Scalar, int Dimensions>
struct Vector
{
    Scalar v[Dimensions];

    Vector() = default;
    Vector(const Scalar* v)
    {
        for (auto i = 0; i < Dimensions; i++)
        {
            this->v[i] = v[i];
        }
    }
    Vector(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();
        for (int i = 0; i < Dimensions; i++)
        {
            this->v[i] = (list.end() == l) ? 0 : *l++;
        }
    }

    inline Scalar& operator[](int index)
    {
        return this->v[index];
    }
    inline const Scalar& operator[](int index) const
    {
        return this->v[index];
    }
    inline operator Scalar*()
    {
        return this->v;
    }
    inline operator const Scalar*() const
    {
        return this->v;
    }
};

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions> operator+(const Vector<Scalar, Dimensions>& first, const Vector<Scalar, Dimensions>& second)
{
    Vector<Scalar, Dimensions> result;

    for (auto i = 0; i < Dimensions; i++)
    {
        result.v[i] = first.v[i] + second.v[i];
    }

    return result;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions> operator-(const Vector<Scalar, Dimensions>& first, const Vector<Scalar, Dimensions>& second)
{
    Vector<Scalar, Dimensions> result;

    for (auto i = 0; i < Dimensions; i++)
    {
        result.v[i] = first.v[i] - second.v[i];
    }

    return result;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions> operator*(const Vector<Scalar, Dimensions>& vector, Scalar scale)
{
    Vector<Scalar, Dimensions> result;

    for (auto i = 0; i < Dimensions; i++)
    {
        result.v[i] = vector.v[i] * scale;
    }

    return result;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions> operator*(Scalar scale, const Vector<Scalar, Dimensions>& vector)
{
    return vector * scale;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions> operator/(const Vector<Scalar, Dimensions>& vector, Scalar scale)
{
    Vector<Scalar, Dimensions> result;

    for (auto i = 0; i < Dimensions; i++)
    {
        result.v[i] = vector.v[i] / scale;
    }

    return result;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions>& operator+=(Vector<Scalar, Dimensions>& first, const Vector<Scalar, Dimensions>& second)
{
    for (auto i = 0; i < Dimensions; i++)
    {
        first.v[i] += second.v[i];
    }

    return first;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions>& operator-=(Vector<Scalar, Dimensions>& first, const Vector<Scalar, Dimensions>& second)
{
    for (auto i = 0; i < Dimensions; i++)
    {
        first.v[i] -= second.v[i];
    }

    return first;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions>& operator+=(Vector<Scalar, Dimensions>& first, Scalar second)
{
    for (auto i = 0; i < Dimensions; i++)
    {
        first.v[i] += second;
    }

    return first;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions>& operator-=(Vector<Scalar, Dimensions>& first, Scalar second)
{
    for (auto i = 0; i < Dimensions; i++)
    {
        first.v[i] -= second;
    }

    return first;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions>& operator*=(Vector<Scalar, Dimensions>& vector, Scalar scale)
{
    for (auto i = 0; i < Dimensions; i++)
    {
        vector.v[i] *= scale;
    }

    return vector;
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions>& operator/=(Vector<Scalar, Dimensions>& vector, Scalar scale)
{
    for (auto i = 0; i < Dimensions; i++)
    {
        vector.v[i] /= scale;
    }

    return vector;
}

template<typename Scalar, int Dimensions>
inline Scalar Dot(const Vector<Scalar, Dimensions>& v0, const Vector<Scalar, Dimensions>& v1)
{
    Scalar product = 0;
    for (int i = 0; i < Dimensions; i++)
    {
        product += v0.v[i] * v1.v[i];
    }
    return product;
}

template<typename Scalar, int Dimensions>
inline Scalar Length(const Vector<Scalar, Dimensions>& v)
{
    return sqrtx(Dot(v, v));
}

template<typename Scalar, int Dimensions>
inline Vector<Scalar, Dimensions> Normalize(const Vector<Scalar, Dimensions>& v)
{
    return v / Length(v);
}

template<typename Scalar>
inline Scalar Dot(const Vector<Scalar, 2>& v0, const Vector<Scalar, 2>& v1)
{
    return v0.v[0] * v1.v[0] + v0.v[1] * v1.v[1];
}

template<typename Scalar>
inline Scalar Dot(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1)
{
    return v0.v[0] * v1.v[0] + v0.v[1] * v1.v[1] + v0.v[2] * v1.v[2];
}

template<typename Scalar>
inline Vector<Scalar, 3> Cross(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1)
{
    return Vector<Scalar, 3>{ v0.v[1] * v1.v[2] - v0.v[2] * v1.v[1],
                              v0.v[2] * v1.v[0] - v0.v[0] * v1.v[2],
                              v0.v[0] * v1.v[1] - v0.v[1] * v1.v[0] };
}

template<typename Scalar>
inline Scalar CosOfVectors(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1)
{
    return Dot(Normalize(v0), Normalize(v1));
}

template<typename Scalar>
struct Vector<Scalar, 2>
{
    union
    {
        Scalar v[2];
        struct
        {
            Scalar X;
            Scalar Y;
        };
    };

    Vector() = default;
    Vector(const Scalar* v)
    {
        for (auto i = 0; i < 2; i++)
        {
            this->v[i] = v[i];
        }
    }
    Vector(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();

        for (int i = 0; i < 2; i++)
        {
            this->v[i] = (list.end() == l) ? 0 : *l++;
        }
    }

    inline Scalar& operator[](int index)
    {
        return this->v[index];
    }
    inline const Scalar& operator[](int index) const
    {
        return this->v[index];
    }
    inline operator Scalar*()
    {
        return this->v;
    }
    inline operator const Scalar*() const
    {
        return this->v;
    }

    inline Scalar Dot() const
    {
        return ::Dot(*this, *this);
    }
    inline Scalar Dot(const Vector<Scalar, 3>& other) const
    {
        return ::Dot(*this, other);
    }
    inline Scalar Length() const
    {
        return ::Length(*this);
    }
    inline Vector<Scalar, 2> Normalize() const
    {
        return ::Normalize(*this);
    }

    static Vector<Scalar, 2> XAxis, YAxis;
};
template<typename Scalar>
Vector<Scalar, 2> Vector<Scalar, 2>::XAxis = { 1, 0 };
template<typename Scalar>
Vector<Scalar, 2> Vector<Scalar, 2>::YAxis = { 0, 1 };

template<typename Scalar>
struct Vector<Scalar, 3>
{
    union
    {
        Scalar v[3];
        struct
        {
            Scalar X;
            Scalar Y;
            Scalar Z;
        };
    };

    Vector() = default;
    Vector(const Scalar* v)
    {
        for (auto i = 0; i < 3; i++)
        {
            this->v[i] = v[i];
        }
    }
    Vector(const std::initializer_list<Scalar>& list)
    {
        auto l = list.begin();

        for (int i = 0; i < 3; i++)
        {
            this->v[i] = (list.end() == l) ? 0 : *l++;
        }
    }

    inline Scalar& operator[](int index)
    {
        return this->v[index];
    }
    inline const Scalar& operator[](int index) const
    {
        return this->v[index];
    }
    inline operator Scalar*()
    {
        return this->v;
    }
    inline operator const Scalar*() const
    {
        return this->v;
    }

    inline Scalar Dot() const
    {
        return ::Dot(*this, *this);
    }
    inline Scalar Dot(const Vector<Scalar, 3>& other) const
    {
        return ::Dot(*this, other);
    }
    inline Scalar Length() const
    {
        return ::Length(*this);
    }
    inline Vector<Scalar, 3> Cross(const Vector<Scalar, 3>& other) const
    {
        return ::Cross(*this, other);
    }
    inline Vector<Scalar, 3> Normalize() const
    {
        return ::Normalize(*this);
    }

    static Vector<Scalar, 3> XAxis, YAxis, ZAxis;
};
template<typename Scalar>
Vector<Scalar, 3> Vector<Scalar, 3>::XAxis = { 1, 0, 0 };
template<typename Scalar>
Vector<Scalar, 3> Vector<Scalar, 3>::YAxis = { 0, 1, 0 };
template<typename Scalar>
Vector<Scalar, 3> Vector<Scalar, 3>::ZAxis = { 0, 0, 1 };

template<typename Scalar>
struct Quaternion : public Vector<Scalar, 4>
{
    Quaternion() = default;
    Quaternion(const Scalar* v) : Vector<Scalar, 4>(v)
    {
    }
    Quaternion(const std::initializer_list<Scalar>& v) : Vector<Scalar, 4>(v)
    {
    }

    inline Vector<Scalar, 3> Rotate(const Vector<Scalar, 3>& v) const
    {
        auto uv = ((Vector<Scalar, 3>*)this)->Cross(v);
        uv += uv;
        return uv * this->v[3] + ((Vector<Scalar, 3>*)this)->Cross(uv) + v;
    }

    inline Quaternion<Scalar> Inverse() const
    {
        return Quaternion<Scalar>{ -this->v[0], -this->v[1], -this->v[2], this->v[3] };
    }

    inline Quaternion<Scalar> operator*(const Quaternion& other) const
    {
        auto& t = (Vector<Scalar, 3>&)*this;
        auto& o = (Vector<Scalar, 3>&)other;

        auto w = t[3] * o[3] - t.Dot(o);
        auto p = t.Cross(o) + o * t[3] + t * o[3];

        return Quaternion<Scalar>{ p.v[0], p.v[1], p.v[2], w };
    }

    inline Vector<Scalar, 4> ToRotation() const
    {
        auto a = acos(this->v[3]);
        auto s = sin(a);

        if (abs(s) < FLT_EPSILON)
        {
            return Vector<Scalar, 4>{ 0, 0, 0, 0 };
        }

        return Vector<Scalar, 4>{ this->v[0] / s, this->v[1] / s, this->v[2] / s, ToDegree(a * 2) };
    }

    inline static Quaternion<Scalar> FromRotation(const Vector<Scalar, 4>& rotation)
    {
        auto& axis = (Vector<Scalar, 3>&)rotation;
        auto angle = rotation.v[3];

        if (axis.Dot() < FLT_EPSILON)
        {
            return Identity;
        }

        return FromAxisAngle(axis, ToRadian(angle));
    }

    inline static Quaternion<Scalar> From2Vectors(const Vector<Scalar, 3>& v0, const Vector<Scalar, 3>& v1)
    {
        auto n0 = v0.Normalize();
        auto n1 = v1.Normalize();

        if (std::fabs(n0[0] - n1[0]) < 0.00001 &&
            std::fabs(n0[1] - n1[1]) < 0.00001 &&
            std::fabs(n0[2] - n1[2]) < 0.00001)
        {
            return Identity;
        }

        auto h = (n0 + n1).Normalize();
        auto q = n0.Cross(h);

        return Quaternion<Scalar>{ q[0], q[1], q[2], n0.Dot(h) };
    }

    inline static Quaternion<Scalar> FromAxisAngle(const Vector<Scalar, 3>& axis, float radian)
    {
        auto a = radian / 2;
        auto c = cos(a);
        auto s = sin(a);
        auto n = axis.Normalize();

        return Quaternion<Scalar>{ s * n.v[0], s * n.v[1], s * n.v[2], c };
    }

    static Quaternion<Scalar> Identity;
};
template<typename Scalar>
Quaternion<Scalar> Quaternion<Scalar>::Identity = { 0, 0, 0, 1 };

typedef Vector<float, 3> Vertex;
typedef Vector<float, 3> Normal;
typedef Vector<float, 2> Coordinate;

template<typename T, size_t N>
char(&_ArraySizeHelper(T(&array)[N]))[N];
#define COUNTOF(array)(sizeof( _ArraySizeHelper(array)))