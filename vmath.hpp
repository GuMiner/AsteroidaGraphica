#pragma once
#define _USE_MATH_DEFINES  1 // Include constants defined in math.h
#include <math.h>

// From the OpenGL Superbible, 6th Ed, 'free to use in your application' as per the website.
// Modified so that quaternions can actually be used and removing dead code.
namespace vmath
{
    template <typename T, const int w, const int h> class matNM;
    template <typename T, const int len> class vecN;
    
    // Goes from radians to degrees
    inline float degrees(float angleInRadians)
    {
        return angleInRadians * (float)(180.0 / M_PI);
    }

    // Goes from degrees to radians
    inline float radians(float angleInDegrees)
    {
        return angleInDegrees * (float)(M_PI / 180.0);
    }

    template <typename T, const int len>
    class vecN
    {
    public:
        typedef class vecN<T, len> my_type;
        typedef T element_type;

        // Default constructor does nothing, just like built-in types
        inline vecN()
        {
            // Uninitialized variable
        }

        // Copy constructor
        inline vecN(const vecN& that)
        {
            assign(that);
        }

        // Construction from scalar
        inline vecN(T s)
        {
            int n;
            for (n = 0; n < len; n++)
            {
                data[n] = s;
            }
        }

        // Assignment operator
        inline vecN& operator=(const vecN& that)
        {
            assign(that);
            return *this;
        }

        inline vecN& operator=(const T& that)
        {
            int n;
            for (n = 0; n < len; n++)
                data[n] = that;

            return *this;
        }

        inline vecN operator+(const vecN& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] + that.data[n];
            return result;
        }

        inline vecN& operator+=(const vecN& that)
        {
            return (*this = *this + that);
        }

        inline vecN operator-() const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = -data[n];
            return result;
        }

        inline vecN operator-(const vecN& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] - that.data[n];
            return result;
        }

        inline vecN& operator-=(const vecN& that)
        {
            return (*this = *this - that);
        }

        inline vecN operator*(const vecN& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] * that.data[n];
            return result;
        }

        inline vecN& operator*=(const vecN& that)
        {
            return (*this = *this * that);
        }

        inline vecN operator*(const T& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] * that;
            return result;
        }

        inline vecN& operator*=(const T& that)
        {
            assign(*this * that);

            return *this;
        }

        inline vecN operator/(const vecN& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] / that.data[n];
            return result;
        }

        inline vecN& operator/=(const vecN& that)
        {
            assign(*this / that);

            return *this;
        }

        inline vecN operator/(const T& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < len; n++)
                result.data[n] = data[n] / that;
            return result;
        }

        inline vecN& operator/=(const T& that)
        {
            assign(*this / that);
            return *this;
        }

        inline T& operator[](int n) { return data[n]; }
        inline const T& operator[](int n) const { return data[n]; }

        inline static int size(void) { return len; }

        inline operator const T* () const { return &data[0]; }
        
    protected:
        T data[len];

        inline void assign(const vecN& that)
        {
            int n;
            for (n = 0; n < len; n++)
                data[n] = that.data[n];
        }
    };

    template <typename T>
    class Tvec3 : public vecN<T, 3>
    {
    public:
        typedef vecN<T, 3> base;

        // Uninitialized variable
        inline Tvec3() {}

        // Copy constructor
        inline Tvec3(const base& v) : base(v) {}

        // vec3(x, y, z);
        inline Tvec3(T x, T y, T z)
        {
            base::data[0] = x;
            base::data[1] = y;
            base::data[2] = z;
        }
    };

    template <typename T>
    class Tvec4 : public vecN<T, 4>
    {
    public:
        typedef vecN<T, 4> base;

        // Uninitialized variable
        inline Tvec4() {}

        // Copy constructor
        inline Tvec4(const base& v) : base(v) {}

        // vec4(x, y, z, w);
        inline Tvec4(T x, T y, T z, T w)
        {
            base::data[0] = x;
            base::data[1] = y;
            base::data[2] = z;
            base::data[3] = w;
        }
    };

    // These types don't exist in GLSL and don't have full implementations
    // (constructors and such). This is enough to get some template functions
    // to compile correctly.
    typedef vecN<float, 1> vec1;
    typedef Tvec3<float> vec3;
    typedef Tvec4<float> vec4;

    template <typename T, int n>
    static inline const vecN<T, n> operator * (T x, const vecN<T, n>& v)
    {
        return v * x;
    }

    template <typename T>
    static inline const Tvec3<T> operator / (T x, const Tvec3<T>& v)
    {
        return Tvec3<T>(x / v[0], x / v[1], x / v[2]);
    }

    template <typename T>
    static inline const Tvec4<T> operator / (T x, const Tvec4<T>& v)
    {
        return Tvec4<T>(x / v[0], x / v[1], x / v[2], x / v[3]);
    }

    template <typename T, int len>
    static inline T dot(const vecN<T, len>& a, const vecN<T, len>& b)
    {
        int n;
        T total = T(0);
        for (n = 0; n < len; n++)
        {
            total += a[n] * b[n];
        }
        return total;
    }

    template <typename T>
    static inline vecN<T, 3> cross(const vecN<T, 3>& a, const vecN<T, 3>& b)
    {
        return Tvec3<T>(a[1] * b[2] - b[1] * a[2],
            a[2] * b[0] - b[2] * a[0],
            a[0] * b[1] - b[0] * a[1]);
    }

    template <typename T, int len>
    static inline T length(const vecN<T, len>& v)
    {
        T result(0);

        for (int i = 0; i < v.size(); ++i)
        {
            result += v[i] * v[i];
        }

        return (T)sqrt(result);
    }

    template <typename T, int len>
    static inline vecN<T, len> normalize(const vecN<T, len>& v)
    {
        return v / length(v);
    }

    template <typename T, int len>
    static inline T distance(const vecN<T, len>& a, const vecN<T, len>& b)
    {
        return length(b - a);
    }

    template <typename T, int len>
    static inline T angle(const vecN<T, len>& a, const vecN<T, len>& b)
    {
        return arccos(dot(a, b));
    }

    template <typename T, const int w, const int h>
    class matNM
    {
    public:
        typedef class matNM<T, w, h> my_type;
        typedef class vecN<T, h> vector_type;

        // Default constructor does nothing, just like built-in types
        inline matNM()
        {
            // Uninitialized variable
        }

        // Copy constructor
        inline matNM(const matNM& that)
        {
            assign(that);
        }

        // Construction from element type
        // explicit to prevent assignment from T
        explicit inline matNM(T f)
        {
            for (int n = 0; n < w; n++)
            {
                data[n] = f;
            }
        }

        // Construction from vector
        inline matNM(const vector_type& v)
        {
            for (int n = 0; n < w; n++)
            {
                data[n] = v;
            }
        }

        // Assignment operator
        inline matNM& operator=(const my_type& that)
        {
            assign(that);
            return *this;
        }

        inline matNM operator+(const my_type& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < w; n++)
                result.data[n] = data[n] + that.data[n];
            return result;
        }

        inline my_type& operator+=(const my_type& that)
        {
            return (*this = *this + that);
        }

        inline my_type operator-(const my_type& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < w; n++)
                result.data[n] = data[n] - that.data[n];
            return result;
        }

        inline my_type& operator-=(const my_type& that)
        {
            return (*this = *this - that);
        }

        inline my_type operator*(const T& that) const
        {
            my_type result;
            int n;
            for (n = 0; n < w; n++)
                result.data[n] = data[n] * that;
            return result;
        }

        inline my_type& operator*=(const T& that)
        {
            int n;
            for (n = 0; n < w; n++)
                data[n] = data[n] * that;
            return *this;
        }

        // Matrix multiply.
        // TODO: This only works for square matrices. Need more template skill to make a non-square version.
        inline my_type operator*(const my_type& that) const
        {
            my_type result(0);

            for (int j = 0; j < w; j++)
            {
                for (int i = 0; i < h; i++)
                {
                    T sum(0);

                    for (int n = 0; n < w; n++)
                    {
                        sum += data[n][i] * that[j][n];
                    }

                    result[j][i] = sum;
                }
            }

            return result;
        }

        inline my_type& operator*=(const my_type& that)
        {
            return (*this = *this * that);
        }

        inline vector_type& operator[](int n) { return data[n]; }
        inline const vector_type& operator[](int n) const { return data[n]; }
        inline operator T*() { return &data[0][0]; }
        inline operator const T*() const { return &data[0][0]; }

        inline matNM<T, h, w> transpose(void) const
        {
            matNM<T, h, w> result;
            int x, y;

            for (y = 0; y < w; y++)
            {
                for (x = 0; x < h; x++)
                {
                    result[x][y] = data[y][x];
                }
            }

            return result;
        }

        static inline my_type identity()
        {
            my_type result(0);

            for (int i = 0; i < w; i++)
            {
                result[i][i] = 1;
            }

            return result;
        }

        static inline int width(void) { return w; }
        static inline int height(void) { return h; }

    protected:
        // Column primary data (essentially, array of vectors)
        vecN<T, h> data[w];

        // Assignment function - called from assignment operator and copy constructor.
        inline void assign(const matNM& that)
        {
            int n;
            for (n = 0; n < w; n++)
                data[n] = that.data[n];
        }
    };

    template <typename T>
    class Tmat4 : public matNM<T, 4, 4>
    {
    public:
        typedef matNM<T, 4, 4> base;
        typedef Tmat4<T> my_type;

        inline Tmat4() {}
        inline Tmat4(const my_type& that) : base(that) {}
        inline Tmat4(const base& that) : base(that) {}
        inline Tmat4(const vecN<T, 4>& v) : base(v) {}
        inline Tmat4(const vecN<T, 4>& v0,
            const vecN<T, 4>& v1,
            const vecN<T, 4>& v2,
            const vecN<T, 4>& v3)
        {
            base::data[0] = v0;
            base::data[1] = v1;
            base::data[2] = v2;
            base::data[3] = v3;
        }
    };

    typedef Tmat4<float> mat4;

    static inline mat4 perspective(float fovy, float aspect, float n, float f)
    {
        float q = 1.0f / tanf(radians(0.5f * fovy));
        float A = q / aspect;
        float B = (n + f) / (n - f);
        float C = (2.0f * n * f) / (n - f);

        mat4 result;

        result[0] = vec4(A, 0.0f, 0.0f, 0.0f);
        result[1] = vec4(0.0f, q, 0.0f, 0.0f);
        result[2] = vec4(0.0f, 0.0f, B, -1.0f);
        result[3] = vec4(0.0f, 0.0f, C, 0.0f);

        return result;
    }

    template <typename T>
    static inline Tmat4<T> translate(T x, T y, T z)
    {
        return Tmat4<T>(Tvec4<T>(1.0f, 0.0f, 0.0f, 0.0f),
            Tvec4<T>(0.0f, 1.0f, 0.0f, 0.0f),
            Tvec4<T>(0.0f, 0.0f, 1.0f, 0.0f),
            Tvec4<T>(x, y, z, 1.0f));
    }

    template <typename T>
    static inline Tmat4<T> translate(const vecN<T, 3>& v)
    {
        return translate(v[0], v[1], v[2]);
    }

    template <typename T>
    static inline Tmat4<T> lookat(const vecN<T, 3>& eye, const vecN<T, 3>& center, const vecN<T, 3>& up)
    {
        const Tvec3<T> f = normalize(center - eye);
        const Tvec3<T> upN = normalize(up);
        const Tvec3<T> s = cross(f, upN);
        const Tvec3<T> u = cross(s, f);
        const Tmat4<T> M = Tmat4<T>(Tvec4<T>(s[0], u[0], -f[0], T(0)),
            Tvec4<T>(s[1], u[1], -f[1], T(0)),
            Tvec4<T>(s[2], u[2], -f[2], T(0)),
            Tvec4<T>(T(0), T(0), T(0), T(1)));

        return M * translate<T>(-eye);
    }

    template <typename T>
    static inline Tmat4<T> scale(T x, T y, T z)
    {
        return Tmat4<T>(Tvec4<T>(x, 0.0f, 0.0f, 0.0f),
            Tvec4<T>(0.0f, y, 0.0f, 0.0f),
            Tvec4<T>(0.0f, 0.0f, z, 0.0f),
            Tvec4<T>(0.0f, 0.0f, 0.0f, 1.0f));
    }

    template <typename T>
    static inline Tmat4<T> scale(const Tvec3<T>& v)
    {
        return scale(v[0], v[1], v[2]);
    }

    template <typename T>
    static inline Tmat4<T> rotate(T angle, T x, T y, T z)
    {
        Tmat4<T> result;

        const T x2 = x * x;
        const T y2 = y * y;
        const T z2 = z * z;
        float rads = float(angle) * 0.0174532925f;
        const float c = cosf(rads);
        const float s = sinf(rads);
        const float omc = 1.0f - c;

        result[0] = Tvec4<T>(T(x2 * omc + c), T(y * x * omc + z * s), T(x * z * omc - y * s), T(0));
        result[1] = Tvec4<T>(T(x * y * omc - z * s), T(y2 * omc + c), T(y * z * omc + x * s), T(0));
        result[2] = Tvec4<T>(T(x * z * omc + y * s), T(y * z * omc - x * s), T(z2 * omc + c), T(0));
        result[3] = Tvec4<T>(T(0), T(0), T(0), T(1));

        return result;
    }

    template <typename T>
    static inline Tmat4<T> rotate(T angle, const vecN<T, 3>& v)
    {
        return rotate<T>(angle, v[0], v[1], v[2]);
    }
    
    const vec3 DEFAULT_FORWARD_VECTOR = vec3(0, 0, -1.0f);
    const vec3 DEFAULT_UP_VECTOR = vec3(0, -1.0f, 0);
    const float NORMALIZE_TOLERANCE = 0.00001f;

    class quaternion
    {
    public:
        inline quaternion()
        {
        }

        // Assignment operator
        inline quaternion& operator=(const quaternion& that)
        {
            x = that.x;
            y = that.y;
            z = that.z;
            w = that.w;
            return *this;
        }

        inline quaternion(const quaternion& that)
        {
            x = that.x;
            y = that.y;
            z = that.z;
            w = that.w;
        }

        inline quaternion(float _x, float _y, float _z, float _w)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        inline quaternion operator*(const quaternion& q) const
        {
            const float x2 = q.x;
            const float y2 = q.y;
            const float z2 = q.z;
            const float w2 = q.w;

            return quaternion(
                w * x2 + x * w2 + y * z2 - z * y2,
                w * y2 + y * w2 + z * x2 - x * z2,
                w * z2 + z * w2 + x * y2 - y * x2,
                w * w2 - x * x2 - y * y2 - z * z2);
        }

        inline void normalize()
        {
            float magnitude = x*x + y*y + z*z + w*w;
            if (fabsf(magnitude - 1) < NORMALIZE_TOLERANCE)
            {
                float magnitudeReal = sqrtf(magnitude);
                x /= magnitudeReal;
                y /= magnitudeReal;
                z /= magnitudeReal;
                w /= magnitudeReal;
            }
        }

        inline quaternion conjugate() const
        {
            return quaternion(-x, -y, -z, w);
        }

        // Given an axis and an angle (in radians), returns a unit quaternion.
        static inline quaternion fromAxisAngle(float angle, Tvec3<float> axis)
        {
            float halfAngle = angle * 0.5f;
            float sinAngle = sinf(angle);

            float x = (axis[0] * sinAngle);
            float y = (axis[1] * sinAngle);
            float z = (axis[2] * sinAngle);
            float w = cosf(angle);

            return quaternion(x, y, z, w);
        }

        inline Tvec3<float> upVector() const
        {
            quaternion resultingVector = *this * (quaternion(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2], 0) * this->conjugate());
            return Tvec3<float>(resultingVector.x, resultingVector.y, resultingVector.z);
        }

        inline Tvec3<float> forwardVector() const
        {
            quaternion resultingVector = *this * (quaternion(DEFAULT_FORWARD_VECTOR[0], DEFAULT_FORWARD_VECTOR[1], DEFAULT_FORWARD_VECTOR[2], 0) * this->conjugate());
            return Tvec3<float>(resultingVector.x, resultingVector.y, resultingVector.z);
        }

        // Returns the Yaw-then-Pitch-then-Roll XYZ Euler Angles from the quaternion, in radians.
        inline Tvec3<float> asEulerAngles() const
        {
            return Tvec3<float>(
                atan2f(2 * (w*x + y*z), 1 - 2 * (x*x + y*y)),
                asinf(2 * (w*y - z*x)),
                atan2f(2 * (w*z + x*y), 1 - 2 * (y*y + z*z)));
        }

        inline matNM<float, 4, 4> asMatrix() const
        {
            matNM<float, 4, 4> m;

            const float xx = x * x;
            const float yy = y * y;
            const float zz = z * z;
            const float ww = w * w;
            const float xy = x * y;
            const float xz = x * z;
            const float xw = x * w;
            const float yz = y * z;
            const float yw = y * w;
            const float zw = z * w;

            m[0][0] = 1.0f - 2.0f * (yy + zz);
            m[0][1] = 2.0f * (xy - zw);
            m[0][2] = 2.0f * (xz + yw);
            m[0][3] = 0.0f;

            m[1][0] = 2.0f * (xy + zw);
            m[1][1] = 1.0f - 2.0f * (xx + zz);
            m[1][2] = 2.0f * (yz - xw);
            m[1][3] = 0.0f;

            m[2][0] = 2.0f * (xz - yw);
            m[2][1] = 2.0f * (yz + xw);
            m[2][2] = 1.0f - 2.0f * (xx + yy);
            m[2][3] = 0.0f;

            m[3][0] = 0.0f;
            m[3][1] = 0.0f;
            m[3][2] = 0.0f;
            m[3][3] = 1.0f;

            return m;
        }

    private:
        float x;
        float y;
        float z;
        float w;
    };

#ifdef min
#undef min
#endif

    template <typename T>
    static inline T min(T a, T b)
    {
        return a < b ? a : b;
    }

#ifdef max
#undef max
#endif

    template <typename T>
    static inline T max(T a, T b)
    {
        return a >= b ? a : b;
    }

    template <typename T, const int N, const int M>
    static inline vecN<T, N> operator*(const vecN<T, M>& vec, const matNM<T, N, M>& mat)
    {
        int n, m;
        vecN<T, N> result(T(0));

        for (m = 0; m < M; m++)
        {
            for (n = 0; n < N; n++)
            {
                result[n] += vec[m] * mat[n][m];
            }
        }

        return result;
    }

    template <typename T, const int N>
    static inline vecN<T, N> operator/(const T s, const vecN<T, N>& v)
    {
        int n;
        vecN<T, N> result;

        for (n = 0; n < N; n++)
        {
            result[n] = s / v[n];
        }

        return result;
    }
};