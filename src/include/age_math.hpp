#ifndef AGE_MATH_HPP
#define AGE_MATH_HPP

#include <cmath>

typedef unsigned int uint;

namespace age
{
    namespace math
	{
		template<typename T>
		struct Vector2
		{
			union
			{
				struct { T x, y; };
				struct { T r, g; };
				struct { T s, t; };
				T raw[2];
			};
			Vector2(T x, T y) { this->x = x; this->y = y; }
			Vector2(T d) : Vector2(d, d) {}
			Vector2() : Vector2(0, 0) {}
			__forceinline T& operator[](size_t index) { return this->raw[index]; }
			__forceinline Vector2& operator+=(Vector2 rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
			__forceinline Vector2& operator+=(T rhs) { this->x += rhs; this->y += rhs; return *this; }
			__forceinline Vector2& operator-=(Vector2 rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
			__forceinline Vector2& operator-=(T rhs) { this->x -= rhs; this->y -= rhs; return *this; }
			__forceinline Vector2& operator*=(Vector2 rhs) { this->x *= rhs.x; this->y *= rhs.y; return *this; }
			__forceinline Vector2& operator*=(T rhs) { this->x *= rhs; this->y *= rhs; return *this; }
			__forceinline Vector2& operator/=(Vector2 rhs) { this->x /= rhs.x; this->y /= rhs.y; return *this; }
			__forceinline Vector2& operator/=(T rhs) { this->x /= rhs; this->y /= rhs; return *this; }
			friend __forceinline Vector2 operator+(Vector2 lhs, Vector2 rhs) { return Vector2(lhs.x + rhs.x, lhs.y + rhs.y); }
			friend __forceinline Vector2 operator+(T lhs, Vector2 rhs) { return Vector2(lhs + rhs.x, lhs + rhs.y); }
			friend __forceinline Vector2 operator+(Vector2 lhs, T rhs) { return Vector2(lhs.x + rhs, lhs.y + rhs); }
			friend __forceinline Vector2 operator-(Vector2 lhs, Vector2 rhs) { return Vector2(lhs.x - rhs.x, lhs.y - rhs.y); }
			friend __forceinline Vector2 operator-(T lhs, Vector2 rhs) { return Vector2(lhs - rhs.x, lhs - rhs.y); }
			friend __forceinline Vector2 operator-(Vector2 lhs, T rhs) { return Vector2(lhs.x - rhs, lhs.y - rhs); }
			friend __forceinline Vector2 operator*(Vector2 lhs, Vector2 rhs) { return Vector2(lhs.x * rhs.x, lhs.y * rhs.y); }
			friend __forceinline Vector2 operator*(T lhs, Vector2 rhs) { return Vector2(lhs * rhs.x, lhs * rhs.y); }
			friend __forceinline Vector2 operator*(Vector2 lhs, T rhs) { return Vector2(lhs.x * rhs, lhs.y * rhs); }
			friend __forceinline Vector2 operator/(Vector2 lhs, Vector2 rhs) { return Vector2(lhs.x / rhs.x, lhs.y / rhs.y); }
			friend __forceinline Vector2 operator/(T lhs, Vector2 rhs) { return Vector2(lhs / rhs.x, lhs / rhs.y); }
			friend __forceinline Vector2 operator/(Vector2 lhs, T rhs) { return Vector2(lhs.x / rhs, lhs.y / rhs); }
			friend __forceinline bool operator==(Vector2 lhs, Vector2 rhs) { return ((lhs.x == rhs.x) && (lhs.y == rhs.y)); }
		};

		template<typename T> __forceinline T dot(Vector2<T> a, Vector2<T> b) { Vector2<T> ab = a * b; return ab.x + ab.y; }
		template<typename T> __forceinline T sqrLength(Vector2<T> v) { return dot(v, v); }
		template<typename F = float, typename T> __forceinline F length(Vector2<T> v)
		{
			static_assert(std::is_floating_point<F>::value, "F should be a floating point number!");
			return sqrt((F)sqrLength(v));
		}
		template<typename F = float, typename T> __forceinline Vector2<F> normalize(Vector2<T> v)
		{
			static_assert(std::is_floating_point<F>::value, "F should be a floating point number!");
			F l = sqrt((F)sqrLength(v));
			return Vector2<F>(((F)v.x) / l, ((F)v.y) / l);
		}

		template<typename T>
		struct Vector3
		{
			union
			{
				struct { T x, y, z; };
				struct { T r, g, b; };
				struct { T s, t, p; };
				T raw[3];
			};
			Vector3(T x, T y, T z) { this->x = x; this->y = y; this->z = z; }
			Vector3(T d) : Vector3(d, d, d) {}
			Vector3() : Vector3(0, 0, 0) {}
			__forceinline T& operator[](size_t index) { return this->raw[index]; }
			__forceinline Vector3& operator+=(Vector3 rhs) { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; return *this; }
			__forceinline Vector3& operator+=(T rhs) { this->x += rhs; this->y += rhs; this->z += rhs; return *this; }
			__forceinline Vector3& operator-=(Vector3 rhs) { this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z; return *this; }
			__forceinline Vector3& operator-=(T rhs) { this->x -= rhs; this->y -= rhs; this->z -= rhs; return *this; }
			__forceinline Vector3& operator*=(Vector3 rhs) { this->x *= rhs.x; this->y *= rhs.y; this->z *= rhs.z; return *this; }
			__forceinline Vector3& operator*=(T rhs) { this->x *= rhs; this->y *= rhs; this->z *= rhs; return *this; }
			__forceinline Vector3& operator/=(Vector3 rhs) { this->x /= rhs.x; this->y /= rhs.y; this->z /= rhs.z; return *this; }
			__forceinline Vector3& operator/=(T rhs) { this->x /= rhs; this->y /= rhs; this->z /= rhs; return *this; }
			friend __forceinline Vector3 operator+(Vector3 lhs, Vector3 rhs) { return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
			friend __forceinline Vector3 operator+(T lhs, Vector3 rhs) { return Vector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z); }
			friend __forceinline Vector3 operator+(Vector3 lhs, T rhs) { return Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }
			friend __forceinline Vector3 operator-(Vector3 lhs, Vector3 rhs) { return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
			friend __forceinline Vector3 operator-(T lhs, Vector3 rhs) { return Vector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }
			friend __forceinline Vector3 operator-(Vector3 lhs, T rhs) { return Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }
			friend __forceinline Vector3 operator*(Vector3 lhs, Vector3 rhs) { return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
			friend __forceinline Vector3 operator*(T lhs, Vector3 rhs) { return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); }
			friend __forceinline Vector3 operator*(Vector3 lhs, T rhs) { return Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
			friend __forceinline Vector3 operator/(Vector3 lhs, Vector3 rhs) { return Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }
			friend __forceinline Vector3 operator/(T lhs, Vector3 rhs) { return Vector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z); }
			friend __forceinline Vector3 operator/(Vector3 lhs, T rhs) { return Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }
			friend __forceinline bool operator==(Vector3 lhs, Vector3 rhs) { return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z)); }
		};

		template<typename T> __forceinline T dot(Vector3<T> a, Vector3<T> b) { Vector3<T> ab = a * b; return ab.x + ab.y + ab.z; }
		template<typename T> __forceinline T sqrLength(Vector3<T> v) { return dot(v, v); }
		template<typename F = float, typename T> __forceinline F length(Vector3<T> v)
		{
			static_assert(std::is_floating_point<F>::value, "F should be a floating point number!");
			return sqrt((F)sqrLength(v));
		}
		template<typename F = float, typename T> __forceinline Vector3<F> normalize(Vector3<T> v)
		{
			static_assert(std::is_floating_point<F>::value, "F should be a floating point number!");
			F l = sqrt((F)sqrLength(v));
			return Vector3<F>(((F)v.x) / l, ((F)v.y) / l, ((F)v.z) / l);
		}

		template<typename T>
		struct Vector4
		{
			union
			{
				struct { T x, y, z, w; };
				struct { T r, g, b, a; };
				struct { T s, t, p, q; };
				T raw[4];
			};
			Vector4(T x, T y, T z, T w) { this->x = x; this->y = y; this->z = z; this->w = w; }
			Vector4(T d) : Vector4(d, d, d, d) {}
			Vector4() : Vector4(0, 0, 0, 0) {}
			__forceinline T& operator[](size_t index) { return this->raw[index]; }
			__forceinline Vector4& operator+=(Vector4 rhs) { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; this->w += rhs.w; return *this; }
			__forceinline Vector4& operator+=(T rhs) { this->x += rhs; this->y += rhs; this->z += rhs; this->w += rhs; return *this; }
			__forceinline Vector4& operator-=(Vector4 rhs) { this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z; this->w -= rhs.w; return *this; }
			__forceinline Vector4& operator-=(T rhs) { this->x -= rhs; this->y -= rhs; this->z -= rhs; this->w -= rhs; return *this; }
			__forceinline Vector4& operator*=(Vector4 rhs) { this->x *= rhs.x; this->y *= rhs.y; this->z *= rhs.z; this->w *= rhs.w; return *this; }
			__forceinline Vector4& operator*=(T rhs) { this->x *= rhs; this->y *= rhs; this->z *= rhs; this->w *= rhs; return *this; }
			__forceinline Vector4& operator/=(Vector4 rhs) { this->x /= rhs.x; this->y /= rhs.y; this->z /= rhs.z; this->w /= rhs.w; return *this; }
			__forceinline Vector4& operator/=(T rhs) { this->x /= rhs; this->y /= rhs; this->z /= rhs; this->w /= rhs; return *this; }
			friend __forceinline Vector4 operator+(Vector4 lhs, Vector4 rhs) { return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
			friend __forceinline Vector4 operator+(T lhs, Vector4 rhs) { return Vector4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w); }
			friend __forceinline Vector4 operator+(Vector4 lhs, T rhs) { return Vector4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }
			friend __forceinline Vector4 operator-(Vector4 lhs, Vector4 rhs) { return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
			friend __forceinline Vector4 operator-(T lhs, Vector4 rhs) { return Vector4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w); }
			friend __forceinline Vector4 operator-(Vector4 lhs, T rhs) { return Vector4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }
			friend __forceinline Vector4 operator*(Vector4 lhs, Vector4 rhs) { return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
			friend __forceinline Vector4 operator*(T lhs, Vector4 rhs) { return Vector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); }
			friend __forceinline Vector4 operator*(Vector4 lhs, T rhs) { return Vector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
			friend __forceinline Vector4 operator/(Vector4 lhs, Vector4 rhs) { return Vector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w); }
			friend __forceinline Vector4 operator/(T lhs, Vector4 rhs) { return Vector4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w); }
			friend __forceinline Vector4 operator/(Vector4 lhs, T rhs) { return Vector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }
			friend __forceinline bool operator==(Vector4 lhs, Vector4 rhs) { return ((lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z) && (lhs.w == rhs.w)); }
		};

		template<typename T> __forceinline T dot(Vector4<T> a, Vector4<T> b) { Vector4<T> ab = a * b; return ab.x + ab.y + ab.z + ab.w; }
		template<typename T> __forceinline T sqrLength(Vector4<T> v) { return dot(v, v); }
		template<typename F = float, typename T> __forceinline F length(Vector4<T> v)
		{
			static_assert(std::is_floating_point<F>::value, "F should be a floating point number!");
			return sqrt((F)sqrLength(v));
		}
		template<typename F = float, typename T> __forceinline Vector4<F> normalize(Vector4<T> v)
		{
			static_assert(std::is_floating_point<F>::value, "F should be a floating point number!");
			F l = sqrt((F)sqrLength(v));
			return Vector4<F>(((F)v.x) / l, ((F)v.y) / l, ((F)v.z) / l, ((F)v.w) / l);
		}

		template<typename T>
		struct Matrix2
		{
			Vector2<T> columns[2];
			Matrix2(Vector2<T> c0, Vector2<T> c1) { this->columns[0] = c0; this->columns[1] = c1; }
			Matrix2(T m00, T m01, T m10, T m11) : Matrix2(Vector2<T>(m00, m01), Vector2<T>(m10, m11)) {}
			Matrix2() : Matrix2(0, 0, 0, 0) {}
			__forceinline Vector2<T>& operator[](size_t index) { return index < 2 ? this->columns[index] : NULL; }
			friend __forceinline Matrix2 transpose(Matrix2 mat) { return Matrix2(mat[0][0], mat[1][0], mat[0][1], mat[1][1]); }
			__forceinline Matrix2& operator+=(Matrix2 rhs) { this[0] += rhs[0]; this[1] += rhs[1]; return *this; }
			__forceinline Matrix2& operator+=(T rhs) { this[0] += rhs; this[1] += rhs; return *this; }
			__forceinline Matrix2& operator-=(Matrix2 rhs) { this[0] -= rhs[0]; this[1] -= rhs[1]; return *this; }
			__forceinline Matrix2& operator-=(T rhs) { this[0] -= rhs; this[1] -= rhs; return *this; }
			__forceinline Matrix2& operator*=(Matrix2 rhs)
			{
				Matrix2 lhs_T = transpose(this);
				this[0][0] = dot(lhs_T[0], rhs[0]);
				this[0][1] = dot(lhs_T[1], rhs[0]);
				this[1][0] = dot(lhs_T[0], rhs[1]);
				this[1][1] = dot(lhs_T[1], rhs[1]);
				return *this;
			}
			__forceinline Matrix2& operator*=(T rhs) { this[0] *= rhs; this[1] *= rhs; return *this; }
			__forceinline Matrix2& operator/=(T rhs) { this[0] /= rhs; this[1] /= rhs; return *this; }
			friend __forceinline Matrix2 operator+(Matrix2 lhs, Matrix2 rhs) { return Matrix2(lhs[0] + rhs[0], lhs[1] + rhs[1]); }
			friend __forceinline Matrix2 operator+(T lhs, Matrix2 rhs) { return Matrix2(lhs + rhs[0], lhs + rhs[1]); }
			friend __forceinline Matrix2 operator+(Matrix2 lhs, T rhs) { return Matrix2(lhs[0] + rhs, lhs[1] + rhs); }
			friend __forceinline Matrix2 operator-(Matrix2 lhs, Matrix2 rhs) { return Matrix2(lhs[0] - rhs[0], lhs[1] - rhs[1]); }
			friend __forceinline Matrix2 operator-(T lhs, Matrix2 rhs) { return Matrix2(lhs - rhs[0], lhs - rhs[1]); }
			friend __forceinline Matrix2 operator-(Matrix2 lhs, T rhs) { return Matrix2(lhs[0] - rhs, lhs[1] - rhs); }
			friend __forceinline Matrix2 operator*(Matrix2 lhs, Matrix2 rhs) {
				Matrix2 lhs_T = transpose(lhs);
				return Matrix2(
					dot(lhs_T[0], rhs[0]), dot(lhs_T[1], rhs[0]),
					dot(lhs_T[0], rhs[1]), dot(lhs_T[1], rhs[1])
				);
			}
			friend __forceinline Matrix2 operator*(T lhs, Matrix2 rhs) { return Matrix2(lhs * rhs[0], lhs * rhs[1]); }
			friend __forceinline Matrix2 operator*(Matrix2 lhs, T rhs) { return Matrix2(lhs[0] * rhs, lhs[1] * rhs); }
			friend __forceinline Vector2<T> operator*(Vector2<T> lhs, Matrix2 rhs) { return Vector2<T>(dot(lhs, rhs[0]), dot(lhs, rhs[1])); }
			friend __forceinline Vector2<T> operator*(Matrix2 lhs, Vector2<T> rhs) { Matrix2 lhs_T = transpose(lhs); return Vector2<T>(dot(lhs_T[0], rhs), dot(lhs_T[1], rhs)); }
			friend __forceinline Matrix2 operator/(Matrix2 lhs, T rhs) { return Matrix2(lhs[0] / rhs, lhs[1] / rhs); }
		};

		template<typename T>
		struct Matrix3
		{
			Vector3<T> columns[3];
			Matrix3(Vector3<T> c0, Vector3<T> c1, Vector3<T> c2) { this->columns[0] = c0; this->columns[1] = c1; this->columns[2] = c2; }
			Matrix3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) : Matrix3(Vector3<T>(m00, m01, m02), Vector3<T>(m10, m11, m12), Vector3<T>(m20, m21, m22)) {}
			Matrix3() : Matrix3(0, 0, 0, 0, 0, 0, 0, 0, 0) {}
			__forceinline Vector3<T>& operator[](size_t index) { return index < 3 ? this->columns[index] : NULL; }
			friend __forceinline Matrix3 transpose(Matrix3 mat) { return Matrix3(mat[0][0], mat[1][0], mat[2][0], mat[0][1], mat[1][1], mat[2][1], mat[0][2], mat[1][2], mat[2][2]); }
			__forceinline Matrix3& operator+=(Matrix3 rhs) { this[0] += rhs[0]; this[1] += rhs[1]; this[2] += rhs[2]; return *this; }
			__forceinline Matrix3& operator+=(T rhs) { this[0] += rhs; this[1] += rhs; this[2] += rhs; return *this; }
			__forceinline Matrix3& operator-=(Matrix3 rhs) { this[0] -= rhs[0]; this[1] -= rhs[1]; this[2] -= rhs[2]; return *this; }
			__forceinline Matrix3& operator-=(T rhs) { this[0] -= rhs; this[1] -= rhs; this[2] -= rhs; return *this; }
			__forceinline Matrix3& operator*=(Matrix3 rhs)
			{
				Matrix3 lhs_T = transpose(this);
				this[0][0] = dot(lhs_T[0], rhs[0]);
				this[0][1] = dot(lhs_T[1], rhs[0]);
				this[0][2] = dot(lhs_T[2], rhs[0]);
				this[1][0] = dot(lhs_T[0], rhs[1]);
				this[1][1] = dot(lhs_T[1], rhs[1]);
				this[1][2] = dot(lhs_T[2], rhs[1]);
				this[2][0] = dot(lhs_T[0], rhs[2]);
				this[2][1] = dot(lhs_T[1], rhs[2]);
				this[2][2] = dot(lhs_T[2], rhs[2]);
				return *this;
			}
			__forceinline Matrix3& operator*=(T rhs) { this[0] *= Vector3<T>(rhs); this[1] *= Vector3<T>(rhs); this[2] *= Vector3<T>(rhs); return *this; }
			__forceinline Matrix3& operator/=(T rhs) { this[0] /= Vector3<T>(rhs); this[1] /= Vector3<T>(rhs); this[2] /= Vector3<T>(rhs); return *this; }
			friend __forceinline Matrix3 operator+(Matrix3 lhs, Matrix3 rhs) { return Matrix3(lhs[0] + rhs[0], lhs[1] + rhs[1], lhs[2] + rhs[2]); }
			friend __forceinline Matrix3 operator+(T lhs, Matrix3 rhs) { return Matrix3(lhs + rhs[0], lhs + rhs[1], lhs + rhs[2]); }
			friend __forceinline Matrix3 operator+(Matrix3 lhs, T rhs) { return Matrix3(lhs[0] + rhs, lhs[1] + rhs, lhs[2] + rhs); }
			friend __forceinline Matrix3 operator-(Matrix3 lhs, Matrix3 rhs) { return Matrix3(lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2]); }
			friend __forceinline Matrix3 operator-(T lhs, Matrix3 rhs) { return Matrix3(lhs - rhs[0], lhs - rhs[1], lhs - rhs[2]); }
			friend __forceinline Matrix3 operator-(Matrix3 lhs, T rhs) { return Matrix3(lhs[0] - rhs, lhs[1] - rhs, lhs[2] - rhs); }
			friend __forceinline Matrix3 operator*(Matrix3 lhs, Matrix3 rhs) {
				Matrix3 lhs_T = transpose(lhs);
				return Matrix3(
					dot(lhs_T[0], rhs[0]), dot(lhs_T[1], rhs[0]), dot(lhs_T[2], rhs[0]),
					dot(lhs_T[0], rhs[1]), dot(lhs_T[1], rhs[1]), dot(lhs_T[2], rhs[1]),
					dot(lhs_T[0], rhs[2]), dot(lhs_T[1], rhs[2]), dot(lhs_T[2], rhs[2])
				);
			}
			friend __forceinline Matrix3 operator*(T lhs, Matrix3 rhs) { return Matrix3(lhs * rhs[0], lhs * rhs[1], lhs * rhs[2]); }
			friend __forceinline Matrix3 operator*(Matrix3 lhs, T rhs) { return Matrix3(lhs[0] * rhs, lhs[1] * rhs, lhs[2] * rhs); }
			friend __forceinline Vector3<T> operator*(Vector3<T> lhs, Matrix3 rhs) { return Vector3<T>(dot(lhs, rhs[0]), dot(lhs, rhs[1]), dot(lhs, rhs[2])); }
			friend __forceinline Vector3<T> operator*(Matrix3 lhs, Vector3<T> rhs) { Matrix3 lhs_T = transpose(lhs); return Vector3<T>(dot(lhs_T[0], rhs), dot(lhs_T[1], rhs), dot(lhs_T[2], rhs)); }
			friend __forceinline Matrix3 operator/(Matrix3 lhs, T rhs) { return Matrix3(lhs[0] / rhs, lhs[1] / rhs, lhs[2] / rhs); }
		};
	}
}

#endif
