#pragma once

#include "defs.hpp"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <boost/numeric/conversion/bounds.hpp>

namespace kd
{
    //
    // Vector2
    //

    template<typename T>
    class Vector2
    {
    public:
        Vector2() : x(0), y(0)
        {
        }

        Vector2(T x, T y) : x(x), y(y)
        {
        }

        explicit Vector2(const T* v) : x(v[0]), y(v[1])
        {
        }

        T length() const
        {
            return std::sqrt(x*x + y*y);
        }

    public:
        T x, y;
    };

    template<typename T>
    inline bool operator==(const Vector2<T>& a, const Vector2<T>& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    template<typename T>
    inline Vector2<T> operator*(const Vector2<T>& a, T s)
    {
        return Vector2<T>(a.x * s, a.y * s);
    }

    template<typename T>
    inline Vector2<T> operator/(const Vector2<T>& a, T s)
    {
        return Vector2<T>(a.x / s, a.y / s);
    }

    template<typename T>
    inline Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
    {
        return Vector2<T>(a.x + b.x, a.y + b.y);
    }

    template<typename T>
    inline Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
    {
        return Vector2<T>(a.x - b.x, a.y - b.y);
    }

    template<typename T>
        inline Vector2<T> operator-(const Vector2<T>& b)
        {
            return Vector2<T>(-b.x, -b.y);
        }

    template<typename T>
        inline T dot(const Vector2<T>& a, const Vector2<T>& b)
        {
            return a.x * b.x + a.y * b.y;
        }

    template<typename T>
        inline T length(const Vector2<T>& a)
        {
            return a.length();
        }

    template<typename T>
        inline Vector2<T> perpendicular(const Vector2<T>& a)
        {
            return Vector2<T>(a.y, -a.x);
        }

    template<typename T>
        inline Vector2<T> normalize(const Vector2<T>& a)
        {
            T len = length(a);
            // TODO: throw exception when len == 0
            return a / len;
        }

    template<typename T>
    static inline bool circle_through_three_points(const Vector2<T> &a, const Vector2<T> &b, const Vector2<T> &c, Vector2<T> &p, float &r)
    {
        T tmp = a.y * (b.x - c.x) + b.y * c.x - b.x * c.y + a.x * (-b.y + c.y);

        if (tmp == 0.0f || tmp * tmp == 0.0f)
            return false;

        T rr = std::sqrt(
                (((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) *
                 ((a.x - c.x) * (a.x - c.x) + (a.y - c.y) * (a.y - c.y)) *
                 ((b.x - c.x) * (b.x - c.x) + (b.y - c.y) * (b.y - c.y))
                 / (tmp * tmp))) / 2.0f;

        tmp *= 2.0f;

        T px =
            (b.y * c.x * c.x +
             -(b.x * b.x + b.y * b.y) * c.y +
             b.y * c.y * c.y +
             a.x * a.x * (-b.y + c.y) +
             a.y * a.y * (-b.y + c.y) +
             a.y * (b.x * b.x + b.y * b.y - c.x * c.x - c.y * c.y)) / tmp;

        T py =
            (a.x * a.x * (b.x - c.x) +
             a.y * a.y * (b.x - c.x) +
             c.x * (b.x * b.x + b.y * b.y - b.x * c.x) +
             -b.x * c.y * c.y +
             a.x * (-b.x * b.x - b.y * b.y + c.x * c.x + c.y * c.y)) / tmp;

        r = rr;
        p.x = px;
        p.y = py;

        return true;
    }

    typedef Vector2<int> Vector2i;
    typedef Vector2<float> Vector2f;
    typedef Vector2<double> Vector2d;

    //
    // Vector3
    //

    template<typename T>
    class Vector3
    {
    public:
        Vector3() : x(0), y(0), z(0)
        {
        }

        Vector3(T x, T y, T z) : x(x), y(y), z(z)
        {
        }

        Vector3(Vector2<T> v, T z) : x(v.x), y(v.y), z(z)
        {
        }

        explicit Vector3(const T* v) : x(v[0]), y(v[1]), z(v[2])
        {
        }

        T length() const
        {
            return std::sqrt(x*x + y*y + z*z);
        }

        T length_squared() const
        {
            return x*x + y*y + z*z;
        }

        Vector2<T> xy() const
        {
            return Vector2<T>(x, y);
        }

        bool normalize()
        {
            T len = length();
            if (len == T(0))
                return false;
            x /= len;
            y /= len;
            z /= len;
            return true;
        }

        T& operator[](int i)
        {
            kd_assert(i >= 0 && i < 3);
            // TODO: potentially dangerous, alignment and stuff...
            return (&x)[i];
        }

        const T& operator[](int i) const
        {
            kd_assert(i >= 0 && i < 3);
            // TODO: potentially dangerous, alignment and stuff...
            return (&x)[i];
        }

    public:
        T x, y, z;
    };

    template<typename T>
    inline bool operator==(const Vector3<T>& a, const Vector3<T>& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    template<typename T>
    inline Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b)
    {
        return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    template<typename T>
    inline Vector3<T>& operator+=(Vector3<T>& a, const Vector3<T>& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        return a;
    }

    template<typename T>
    inline Vector3<T>& operator-=(Vector3<T>& a, const Vector3<T>& b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
        return a;
    }

    template<typename T>
    inline Vector3<T>& operator*=(Vector3<T>& a, T s)
    {
        a.x *= s;
        a.y *= s;
        a.z *= s;
        return a;
    }

    template<typename T>
    inline Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b)
    {
        return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    template<typename T>
    inline Vector3<T> operator-(const Vector3<T>& b)
    {
        return Vector3<T>(-b.x, -b.y, -b.z);
    }

    template<typename T>
    inline Vector3<T> scale(const Vector3<T>& a, const Vector3<T>& b)
    {
        return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    template<typename T>
    inline Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b)
    {
        return scale(a, b);
    }

    template<typename T>
    inline Vector3<T> operator*(const Vector3<T>& a, T s)
    {
        return Vector3<T>(a.x * s, a.y * s, a.z * s);
    }

    template<typename T>
    inline Vector3<T> operator/(const Vector3<T>& a, T s)
    {
        return Vector3<T>(a.x / s, a.y / s, a.z / s);
    }

    template<typename T>
    inline T dot(const Vector3<T>& a, const Vector3<T>& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    template<typename T>
    inline Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b)
    {
        return Vector3<T>(
                a.y * b.z - a.z * b.y,
                a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
    }

    template<typename T>
    inline Vector3<T> normalize(const Vector3<T>& a)
    {
        Vector3<T> b = a;
        // Is this a bad idea?
        if (!b.normalize())
            throw std::runtime_error("bad vector to normalize");
        return b;
    }

    template<typename T>
    inline T length(const Vector3<T>& v)
    {
        return v.length();
    }

    template<typename T>
    inline T length2(const Vector3<T>& v)
    {
        return v.length_squared();
    }

    template<typename T>
    inline Vector3<T> min_values(const Vector3<T>& a, const Vector3<T>& b)
    {
        return Vector3<T>(
            a.x < b.x ? a.x : b.x,
            a.y < b.y ? a.y : b.y,
            a.z < b.z ? a.z : b.z);
    }

    template<typename T>
    inline Vector3<T> max_values(const Vector3<T>& a, const Vector3<T>& b)
    {
        return Vector3<T>(
            a.x >= b.x ? a.x : b.x,
            a.y >= b.y ? a.y : b.y,
            a.z >= b.z ? a.z : b.z);
    }

    template<typename A, typename B>
    inline Vector3<A> convert_type(const Vector3<B>& v)
    {
        return Vector3<A>(A(v.x), A(v.y), A(v.z));
    }

    typedef Vector3<int> Vector3i;
    typedef Vector3<float> Vector3f;
    typedef Vector3<double> Vector3d;

    //
    // Vector4
    //

    template<typename T>
    class Vector4
    {
    public:
        Vector4() : x(0), y(0), z(0), w(0)
        {
        }

        Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
        {
        }

        Vector4(const Vector3<T>& v, T w) : x(v.x), y(v.y), z(v.z), w(w)
        {
        }

        Vector4(const Vector2<T>& v, T z, T w) : x(v.x), y(v.y), z(z), w(w)
        {
        }

        Vector3<T> xyz() const
        {
            return Vector3<T>(x, y, z);
        }

        Vector2<T> xy() const
        {
            return Vector2<T>(x, y);
        }

        T length() const
        {
            return std::sqrt(x*x + y*y + z*z + w*w);
        }

        Vector3<T> project() const
        {
            return Vector3<T>(x / w, y / w, z / w);
        }

        T& operator[](int i)
        {
            kd_assert(i >= 0 && i < 4);
            // TODO: potentially dangerous, alignment and stuff...
            return (&x)[i];
        }

        const T& operator[](int i) const
        {
            kd_assert(i >= 0 && i < 4);
            // TODO: potentially dangerous, alignment and stuff...
            return (&x)[i];
        }

    public:
        T x, y, z, w;
    };

    template<typename T>
    inline T length(const Vector4<T>& v)
    {
        return v.length();
    }

    template<typename T>
    inline T dot(const Vector4<T>& a, const Vector4<T>& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    template<typename T>
    inline T dot(const Vector4<T>& a, const Vector3<T>& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w;
    }

    template<typename T>
    inline Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b)
    {
        return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    template<typename T>
    inline Vector4<T> operator*(const Vector4<T>& a, T s)
    {
        return Vector4<T>(a.x*s, a.y*s, a.z*s, a.w*s);
    }

    template<typename T>
    inline Vector4<T>& operator+=(Vector4<T>& a, const Vector4<T>& b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
        a.w += b.w;
        return a;
    }

    template<typename T>
    inline Vector4<T>& operator/=(Vector4<T>& a, T d)
    {
        a.x /= d;
        a.y /= d;
        a.z /= d;
        a.w /= d;
        return a;
    }

    template<typename T>
    inline bool operator==(const Vector4<T>& a, const Vector4<T>& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    template<typename T>
    inline Vector3<T> project(const Vector4<T>& v)
    {
        return Vector3<T>(v.x/v.w, v.y/v.w, v.z/v.w);
    }

    typedef Vector4<int> Vector4i;
    typedef Vector4<float> Vector4f;
    typedef Vector4<double> Vector4d;

    //
    // Matrix4x4
    //

    /*  0  1  2  3
     *  4  5  6  7
     *  8  9 10 11
     * 12 13 14 15
     *
     * row-major
     */
    template<typename T>
    class Matrix4x4
    {
    public:
        Matrix4x4()
        {
            identity();
        }

        explicit Matrix4x4(const T* a)
        {
            for (int i = 0; i < 16; i++)
                m[i] = a[i];
        }

        bool is_identity()
        {
            return m[0]  == T(1) && m[1]  == T(0) && m[2]  == T(0) && m[3]  == T(0) &&
                   m[4]  == T(0) && m[5]  == T(1) && m[6]  == T(0) && m[7]  == T(0) &&
                   m[8]  == T(0) && m[9]  == T(0) && m[10] == T(1) && m[11] == T(0) &&
                   m[12] == T(0) && m[13] == T(0) && m[14] == T(0) && m[15] == T(1);
        }

        void identity()
        {
            for (int i = 0; i < 16; i++)
                m[i] = T(0);
            m[0] = m[5] = m[10] = m[15] = T(1);
        }

        void transpose()
        {
            std::swap(m[1], m[4]);
            std::swap(m[2], m[8]);
            std::swap(m[3], m[12]);
            std::swap(m[6], m[9]);
            std::swap(m[7], m[13]);
            std::swap(m[11], m[14]);
        }

        void scale(T s)
        {
            for (int i = 0; i < 16; i++)
                m[i] *= s;
        }

        T determinant()
        {
            return
                get(0, 3) * get(1, 2) * get(2, 1) * get(3, 0) -
                get(0, 2) * get(1, 3) * get(2, 1) * get(3, 0) -
                get(0, 3) * get(1, 1) * get(2, 2) * get(3, 0) +
                get(0, 1) * get(1, 3) * get(2, 2) * get(3, 0) +
                get(0, 2) * get(1, 1) * get(2, 3) * get(3, 0) -
                get(0, 1) * get(1, 2) * get(2, 3) * get(3, 0) -
                get(0, 3) * get(1, 2) * get(2, 0) * get(3, 1) +
                get(0, 2) * get(1, 3) * get(2, 0) * get(3, 1) +
                get(0, 3) * get(1, 0) * get(2, 2) * get(3, 1) -
                get(0, 0) * get(1, 3) * get(2, 2) * get(3, 1) -
                get(0, 2) * get(1, 0) * get(2, 3) * get(3, 1) +
                get(0, 0) * get(1, 2) * get(2, 3) * get(3, 1) +
                get(0, 3) * get(1, 1) * get(2, 0) * get(3, 2) -
                get(0, 1) * get(1, 3) * get(2, 0) * get(3, 2) -
                get(0, 3) * get(1, 0) * get(2, 1) * get(3, 2) +
                get(0, 0) * get(1, 3) * get(2, 1) * get(3, 2) +
                get(0, 1) * get(1, 0) * get(2, 3) * get(3, 2) -
                get(0, 0) * get(1, 1) * get(2, 3) * get(3, 2) -
                get(0, 2) * get(1, 1) * get(2, 0) * get(3, 3) +
                get(0, 1) * get(1, 2) * get(2, 0) * get(3, 3) +
                get(0, 2) * get(1, 0) * get(2, 1) * get(3, 3) -
                get(0, 0) * get(1, 2) * get(2, 1) * get(3, 3) -
                get(0, 1) * get(1, 0) * get(2, 2) * get(3, 3) +
                get(0, 0) * get(1, 1) * get(2, 2) * get(3, 3);
        }

        bool invert()
        {
            T det = determinant();
            if (det == 0)
                return false;
            T r[16];
            r[0*4+0] = get(1, 2)*get(2, 3)*get(3, 1) -
                get(1, 3)*get(2, 2)*get(3, 1) +
                get(1, 3)*get(2, 1)*get(3, 2) -
                get(1, 1)*get(2, 3)*get(3, 2) -
                get(1, 2)*get(2, 1)*get(3, 3) +
                get(1, 1)*get(2, 2)*get(3, 3);
            r[0*4+1] = get(0, 3)*get(2, 2)*get(3, 1) -
                get(0, 2)*get(2, 3)*get(3, 1) -
                get(0, 3)*get(2, 1)*get(3, 2) +
                get(0, 1)*get(2, 3)*get(3, 2) +
                get(0, 2)*get(2, 1)*get(3, 3) -
                get(0, 1)*get(2, 2)*get(3, 3);
            r[0*4+2] = get(0, 2)*get(1, 3)*get(3, 1) -
                get(0, 3)*get(1, 2)*get(3, 1) +
                get(0, 3)*get(1, 1)*get(3, 2) -
                get(0, 1)*get(1, 3)*get(3, 2) -
                get(0, 2)*get(1, 1)*get(3, 3) +
                get(0, 1)*get(1, 2)*get(3, 3);
            r[0*4+3] = get(0, 3)*get(1, 2)*get(2, 1) -
                get(0, 2)*get(1, 3)*get(2, 1) -
                get(0, 3)*get(1, 1)*get(2, 2) +
                get(0, 1)*get(1, 3)*get(2, 2) +
                get(0, 2)*get(1, 1)*get(2, 3) -
                get(0, 1)*get(1, 2)*get(2, 3);
            r[1*4+0] = get(1, 3)*get(2, 2)*get(3, 0) -
                get(1, 2)*get(2, 3)*get(3, 0) -
                get(1, 3)*get(2, 0)*get(3, 2) +
                get(1, 0)*get(2, 3)*get(3, 2) +
                get(1, 2)*get(2, 0)*get(3, 3) -
                get(1, 0)*get(2, 2)*get(3, 3);
            r[1*4+1] = get(0, 2)*get(2, 3)*get(3, 0) -
                get(0, 3)*get(2, 2)*get(3, 0) +
                get(0, 3)*get(2, 0)*get(3, 2) -
                get(0, 0)*get(2, 3)*get(3, 2) -
                get(0, 2)*get(2, 0)*get(3, 3) +
                get(0, 0)*get(2, 2)*get(3, 3);
            r[1*4+2] = get(0, 3)*get(1, 2)*get(3, 0) -
                get(0, 2)*get(1, 3)*get(3, 0) -
                get(0, 3)*get(1, 0)*get(3, 2) +
                get(0, 0)*get(1, 3)*get(3, 2) +
                get(0, 2)*get(1, 0)*get(3, 3) -
                get(0, 0)*get(1, 2)*get(3, 3);
            r[1*4+3] = get(0, 2)*get(1, 3)*get(2, 0) -
                get(0, 3)*get(1, 2)*get(2, 0) +
                get(0, 3)*get(1, 0)*get(2, 2) -
                get(0, 0)*get(1, 3)*get(2, 2) -
                get(0, 2)*get(1, 0)*get(2, 3) +
                get(0, 0)*get(1, 2)*get(2, 3);
            r[2*4+0] = get(1, 1)*get(2, 3)*get(3, 0) -
                get(1, 3)*get(2, 1)*get(3, 0) +
                get(1, 3)*get(2, 0)*get(3, 1) -
                get(1, 0)*get(2, 3)*get(3, 1) -
                get(1, 1)*get(2, 0)*get(3, 3) +
                get(1, 0)*get(2, 1)*get(3, 3);
            r[2*4+1] = get(0, 3)*get(2, 1)*get(3, 0) -
                get(0, 1)*get(2, 3)*get(3, 0) -
                get(0, 3)*get(2, 0)*get(3, 1) +
                get(0, 0)*get(2, 3)*get(3, 1) +
                get(0, 1)*get(2, 0)*get(3, 3) -
                get(0, 0)*get(2, 1)*get(3, 3);
            r[2*4+2] = get(0, 1)*get(1, 3)*get(3, 0) -
                get(0, 3)*get(1, 1)*get(3, 0) +
                get(0, 3)*get(1, 0)*get(3, 1) -
                get(0, 0)*get(1, 3)*get(3, 1) -
                get(0, 1)*get(1, 0)*get(3, 3) +
                get(0, 0)*get(1, 1)*get(3, 3);
            r[2*4+3] = get(0, 3)*get(1, 1)*get(2, 0) -
                get(0, 1)*get(1, 3)*get(2, 0) -
                get(0, 3)*get(1, 0)*get(2, 1) +
                get(0, 0)*get(1, 3)*get(2, 1) +
                get(0, 1)*get(1, 0)*get(2, 3) -
                get(0, 0)*get(1, 1)*get(2, 3);
            r[3*4+0] = get(1, 2)*get(2, 1)*get(3, 0) -
                get(1, 1)*get(2, 2)*get(3, 0) -
                get(1, 2)*get(2, 0)*get(3, 1) +
                get(1, 0)*get(2, 2)*get(3, 1) +
                get(1, 1)*get(2, 0)*get(3, 2) -
                get(1, 0)*get(2, 1)*get(3, 2);
            r[3*4+1] = get(0, 1)*get(2, 2)*get(3, 0) -
                get(0, 2)*get(2, 1)*get(3, 0) +
                get(0, 2)*get(2, 0)*get(3, 1) -
                get(0, 0)*get(2, 2)*get(3, 1) -
                get(0, 1)*get(2, 0)*get(3, 2) +
                get(0, 0)*get(2, 1)*get(3, 2);
            r[3*4+2] = get(0, 2)*get(1, 1)*get(3, 0) -
                get(0, 1)*get(1, 2)*get(3, 0) -
                get(0, 2)*get(1, 0)*get(3, 1) +
                get(0, 0)*get(1, 2)*get(3, 1) +
                get(0, 1)*get(1, 0)*get(3, 2) -
                get(0, 0)*get(1, 1)*get(3, 2);
            r[3*4+3] = get(0, 1)*get(1, 2)*get(2, 0) -
                get(0, 2)*get(1, 1)*get(2, 0) +
                get(0, 2)*get(1, 0)*get(2, 1) -
                get(0, 0)*get(1, 2)*get(2, 1) -
                get(0, 1)*get(1, 0)*get(2, 2) +
                get(0, 0)*get(1, 1)*get(2, 2);
            *this = Matrix4x4<T>(r);
            scale(T(1) / det);
            return true;
        }

        inline Vector4<T> row(int i) const
        {
            kd_assert(i >= 0 && i < 4);
            return Vector4<T>(m[i*4+0], m[i*4+1], m[i*4+2], m[i*4+3]);
        }

        inline Vector4<T> column(int i) const
        {
            kd_assert(i >= 0 && i < 4);
            return Vector4<T>(m[0*4+i], m[1*4+i], m[2*4+i], m[3*4+i]);
        }

        inline T get(int i, int j) const
        {
            kd_assert(i >= 0 && i < 4 && j >= 0 && j < 4);
            return m[i*4+j];
        }

        const T* data() const
        {
            return m;
        }

        T* data()
        {
            return m;
        }

        const T& operator[](int i) const
        {
            kd_assert(i >= 0 && i < 16);
            return m[i];
        }

        T& operator[](int i)
        {
            kd_assert(i >= 0 && i < 16);
            return m[i];
        }

    private:
        T m[16];
    };

    template<typename T>
    Vector4<T> operator*(const Matrix4x4<T>& m, const Vector4<T>& v)
    {
        return Vector4<T>(
            m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
            m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
            m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
            m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w);
    }

    template<typename T>
    Vector3<T> operator*(const Matrix4x4<T>& m, const Vector3<T>& v)
    {
        return Vector3<T>(
            m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3],
            m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7],
            m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11]);
    }

    template<typename T>
    Matrix4x4<T> operator*(const Matrix4x4<T>& a, const Matrix4x4<T>& b)
    {
        Matrix4x4<T> r;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                r[i*4+j] = dot(a.row(i), b.column(j));
            }
        }

        return r;
    }

    template<typename T>
    Matrix4x4<T> transpose(const Matrix4x4<T>& m)
    {
        Matrix4x4<T> m2 = m;
        m2.transpose();
        return m2;
    }

    template<typename T>
    Matrix4x4<T> invert(const Matrix4x4<T>& m)
    {
        Matrix4x4<T> m2 = m;
        if (!m2.invert())
        {
            throw std::runtime_error("matrix is not invertible");
        }
        return m2;
    }

    template<typename T>
    Matrix4x4<T> rotate(const Vector3<T>& vec, T rad)
    {
        // This formula is from OpenGL manpages.

        Vector3<T> v = normalize(vec);
        T x = v.x;
        T y = v.y;
        T z = v.z;
        T c = std::cos(rad);
        T s = std::sin(rad);

        Matrix4x4<T> m;

        m[0]  = x * x * (T(1) - c) + c;
        m[1]  = x * y * (T(1) - c) - z * s;
        m[2]  = x * z * (T(1) - c) + y * s;

        m[4]  = y * x * (T(1) - c) + z * s;
        m[5]  = y * y * (T(1) - c) + c;
        m[6]  = y * z * (T(1) - c) - x * s;

        m[8]  = z * x * (T(1) - c) - y * s;
        m[9]  = z * y * (T(1) - c) + x * s;
        m[10] = z * z * (T(1) - c) + c;

        //m.transpose();

        return m;
    }

    template<typename T>
    Matrix4x4<T> translate(const Vector3<T>& vec)
    {
        Matrix4x4<T> m;

        m[3] = vec.x;
        m[7] = vec.y;
        m[11] = vec.z;

        //m.transpose();

        return m;
    }

    template<typename T>
    Matrix4x4<T> scale(const Vector3<T>& vec)
    {
        Matrix4x4<T> m;

        m[0] = vec.x;
        m[5] = vec.y;
        m[10] = vec.z;

        return m;
    }

    template<typename T>
    Matrix4x4<T> perspective(T fovy, T aspect, T zNear, T zFar)
    {
        kd_assert(zNear > T(0) && zNear < zFar);

        Matrix4x4<T> m;
        m.identity();

        T f = T(1) / std::tan(fovy / T(2));

        m[0*4+0] = f / aspect;
        m[1*4+1] = f;
        m[2*4+2] = (zFar + zNear) / (zNear - zFar);
        m[2*4+3] = T(2) * zFar * zNear / (zNear - zFar);
        m[3*4+2] = T(-1);
        m[3*4+3] = T(0);

        return m;
    }

    template<typename T>
    Matrix4x4<T> ortho(const Vector3<T>& p0, const Vector3<T>& p1)
    {
        T left = p0.x;
        T right = p1.x;
        T bottom = p0.y;
        T top = p1.y;
        T nearval = p0.z;
        T farval = p1.z;

        Matrix4x4<T> m;
        m[0] = T(2) / (right - left);
        m[5] = T(2) / (top - bottom);
        m[10] = T(2) / (farval - nearval);
        m[3] = -(right + left) / (right - left);
        m[7] = -(top + bottom) / (top - bottom);
        m[11] = -(farval + nearval) / (farval - nearval);

        return m;
    }

    template<typename T>
    Matrix4x4<T> ortho2d(const Vector2<T>& p0, const Vector2<T>& p1)
    {
        return ortho<T>(Vector3<T>(p0, T(-1)), Vector3<T>(p1, T(1)));
    }

    template<typename T>
    Matrix4x4<T> look_at(const Vector3<T>& eye, const Vector3<T>& center, const Vector3<T>& up)
    {
        Vector3<T> f = normalize(center - eye);
        Vector3<T> s = normalize(cross(f, up));
        Vector3<T> u = cross(s, f);

        Matrix4x4<T> m;
        m[0] = s.x;
        m[1] = s.y;
        m[2] = s.z;
        m[4] = u.x;
        m[5] = u.y;
        m[6] = u.z;
        m[8] = -f.x;
        m[9] = -f.y;
        m[10] = -f.z;

        return m * translate(-eye);
    }

    template<typename T, typename F>
    Matrix4x4<T> convert_to(const Matrix4x4<F>& m)
    {
        Matrix4x4<T> r;

        for (int i = 0; i < 16; i++)
            r[i] = T(m[i]);

        return r;
    }

    typedef Matrix4x4<float> Matrix4x4f;
    typedef Matrix4x4<double> Matrix4x4d;
    typedef Matrix4x4<float> Matrix4f;
    typedef Matrix4x4<double> Matrix4d;

    //
    // Quaternion
    //

    template<typename T>
    class Quaternion
    {
    public:
        Quaternion() {}

        Quaternion(const Vector4<T>& v) : v(v) {}

        Quaternion(const T& x, const T& y, const T& z, const T& w)
        {
            v = Vector4<T>(x, y, z, w);
        }

        T length() const
        {
            return v.length();
        }

        T length_squared() const
        {
            return dot(v, v);
        }

        Vector4<T> v;
    };

    template<typename T>
    Quaternion<T> operator+(const Quaternion<T>& a, const Quaternion<T>& b)
    {
        return Quaternion<T>(a.v + b.v);
    }

    template<typename T>
    Quaternion<T> operator*(const Quaternion<T>& qa, const Quaternion<T>& qb)
    {
        const Vector4<T>& a = qa.v;
        const Vector4<T>& b = qb.v;

        T x = a.x*b.x - a.y*b.y - a.z*b.z - a.w*b.w;
        T y = a.x*b.y + a.y*b.x + a.z*b.w - a.w*b.z;
        T z = a.x*b.z - a.y*b.w + a.z*b.x + a.w*b.y;
        T w = a.x*b.w + a.y*b.z - a.z*b.y + a.w*b.x;

        return Quaternion<T>(x, y, z, w);
    }

    typedef Quaternion<float>  Quaternionf;
    typedef Quaternion<double> Quaterniond;

    //
    // AABB
    //

    template<typename T>
    class AABB
    {
    public:
        AABB()
        {
            min.x = boost::numeric::bounds<T>::highest();
            min.y = boost::numeric::bounds<T>::highest();
            min.z = boost::numeric::bounds<T>::highest();
            max.x = boost::numeric::bounds<T>::lowest();
            max.y = boost::numeric::bounds<T>::lowest();
            max.z = boost::numeric::bounds<T>::lowest();
        }

        AABB(const Vector3<T>& a, const Vector3<T>& b)
        :   min(a), max(b)
        {
        }

        void grow(const Vector3<T>& v)
        {
            min.x = std::min(min.x, v.x);
            min.y = std::min(min.y, v.y);
            min.z = std::min(min.z, v.z);
            max.x = std::max(max.x, v.x);
            max.y = std::max(max.y, v.y);
            max.z = std::max(max.z, v.z);
        }

        void grow(const AABB<T>& a)
        {
            grow(a.min);
            grow(a.max);
        }

        void inflate(const Vector3<T>& v)
        {
            grow(min-v);
            grow(max+v);
        }

        void intersection(const AABB<T>& a)
        {
            min.x = std::max(min.x, a.min.x);
            min.y = std::max(min.y, a.min.y);
            min.z = std::max(min.z, a.min.z);
            max.x = std::min(max.x, a.max.x);
            max.y = std::min(max.y, a.max.y);
            max.z = std::min(max.z, a.max.z);
        }

        bool intersects(const AABB<T>& a) const
        {
            AABB<T> aabb = *this;
            aabb.intersection(a);
            return aabb.isValid();
        }

        // NOTE: volume intersection is always area intersection
        bool intersectsWithArea(const AABB<T>& a) const
        {
            AABB<T> aabb = *this;
            aabb.intersection(a);
            if (!aabb.isValid())
                return false;

            return (aabb.min.x < aabb.max.x) + (aabb.min.y < aabb.max.y) + (aabb.min.z < aabb.max.z) >= 2;
        }

        bool intersectsWithVolume(const AABB<T>& a) const
        {
            AABB<T> aabb = *this;
            intersection(aabb, a);
            return aabb.min.x < aabb.max.x && aabb.min.y < aabb.max.y && aabb.min.z < aabb.max.z;
        }

        bool contains(const Vector3<T>& v) const
        {
            return
                v.x >= min.x && v.x <= max.x &&
                v.y >= min.y && v.y <= max.y &&
                v.z >= min.z && v.z <= max.z;
        }

        Vector3<T> getCenter() const
        {
            return Vector3<T>((min.x+max.x)/T(2), (min.y+max.y)/T(2), (min.z+max.z)/T(2));
        }

        Vector3<T> getDiagonal() const
        {
            return max - min;
        }

        T getVolume() const
        {
            Vector3<T> diag = getDiagonal();
            return diag.x * diag.y * diag.z;
        }

        T getSurfaceArea() const
        {
            Vector3f diag = getDiagonal();
            return T(2) * (diag.x * diag.y + diag.x * diag.z + diag.y * diag.z);
        }

        AABB<T> getSubAABB(int x, int y, int z, int w, int h, int d) const
        {
            AABB<T> aabb;

            T fx1 = T(x) / T(w);
            T fy1 = T(y) / T(h);
            T fz1 = T(z) / T(d);
            T fx2 = T(x+1) / T(w);
            T fy2 = T(y+1) / T(h);
            T fz2 = T(z+1) / T(d);

            aabb.min.x = max.x * fx1 + min.x * (T(1) - fx1);
            aabb.min.y = max.y * fy1 + min.y * (T(1) - fy1);
            aabb.min.z = max.z * fz1 + min.z * (T(1) - fz1);
            aabb.max.x = max.x * fx2 + min.x * (T(1) - fx2);
            aabb.max.y = max.y * fy2 + min.y * (T(1) - fy2);
            aabb.max.z = max.z * fz2 + min.z * (T(1) - fz2);

            return aabb;
        }

        AABB<T> getOctreeAABB(int i) const
        {
            kd_assert(i >= 0 && i < 8);
            return getSubAABB(!!(i&1), !!(i&2), !!(i&4), 2, 2, 2);
        }

        Vector3<T> getCorner(int i) const
        {
            kd_assert(i >= 0 && i < 8);
            return Vector3<T>(i&1 ? max.x : min.x, i&2 ? max.y : min.y, i&4 ? max.z : min.z);
        }

        void getFace(int f, Vector3<T>* p) const
        {
            kd_assert(f >= 0 && f < 6);
            int axis = f >> 1;
            int dir = f & 1;

            for (int i = 0; i < 4; i++)
            {
                int j = dir << axis;
                j |= (i&1) << ((axis+1)%3);
                j |= ((i&2)>>1) << ((axis+2)%3);
                p[i] = getCorner(j);
            }
        }

        int getLongestAxis() const
        {
            Vector3<T> diag = getDiagonal();
            if (diag.x >= diag.y && diag.x >= diag.z)
                return 0;
            if (diag.y >= diag.x && diag.y >= diag.z)
                return 1;
            return 2;
        }

        T getLongestAxisLength() const
        {
            Vector3<T> diag = getDiagonal();
            if (diag.x >= diag.y && diag.x >= diag.z)
                return diag.x;
            if (diag.y >= diag.x && diag.y >= diag.z)
                return diag.y;
            return diag.z;
        }

        bool isValid() const
        {
            return min.x <= max.x && min.y <= max.y && min.z <= max.z;
        }

    public:
        Vector3<T> min, max;
    };

    typedef AABB<float> AABBf;
    typedef AABB<double> AABBd;

    inline int faceToIndexMask(int face)
    {
        int axis = face >> 1;
        int dir = face & 1;
        int m = 0;

        for (int i = 0; i < 4; i++)
        {
            int j = dir << axis;
            j |= (i&1) << ((axis+1)%3);
            j |= ((i&2)>>1) << ((axis+2)%3);
            m |= 1 << j;
        }
        return m;
    }

    //
    // Sampling and pseudo-random.
    //

    template<typename T, int P>
    static inline T halton(int k)
    {
        int pp = P;
        int kk = k + 1;
        T res = T(0);

        while (kk > 0)
        {
            int a = kk % P;
            res += T(a) / pp;
            kk /= P;
            pp *= P;
        }

        return res;
    }

    template<int P> static inline float  haltonf(int k) { return halton<float, P>(k); }
    template<int P> static inline double haltond(int k) { return halton<double, P>(k); }

    static const double PI = 3.14159265358979323846;
    static const float PIf = 3.14159265358979323846f;

    template<typename T>
    static inline Vector2<T> uniform_disc(T x, T y)
    {
        kd_assert(x >= T(0) && x <= T(1));
        asserrt(y >= T(0) && y <= T(1));

        T phi = x * T(2) * T(PI);
        T dist = std::sqrt(y);

        return Vector2<T>(std::cos(phi)*dist, std::sin(phi)*dist);
    }

    template<typename T>
    static inline Vector3<T> point_on_sphere(T u, T v)
    {
        u *= T(2) * T(PI);
        v = v * T(2) - T(1);

        T a = std::sqrt(T(1) - v*v);

        return Vector3<T>(std::cos(u)*a, std::sin(u)*a, v);
    }

    //
    // Misc.
    //

    static inline float toRadians(float f)
    {
        return f * 2.f * PIf / 360.f;
    }

    static inline float degToRad(float f)
    {
        return f * 2.f * PIf / 360.f;
    }

    static inline float toDegrees(float f)
    {
        return f / (2.f * PIf) * 360.f;
    }

    static inline float radToDeg(float f)
    {
        return f / (2.f * PIf) * 360.f;
    }

    static inline int prevMod3(int x) { return (0x12 >> (2*x)) & ~3; }
    static inline int nextMod3(int x) { return (0x09 >> (2*x)) & ~3; }
}
