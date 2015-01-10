#pragma once

#ifndef OPMATH_MAT4
#define OPMATH_MAT4
#include "./Math/include/OPvec2.h"
#include "./Math/include/OPvec3.h"
#include "./Math/include/OPvec4.h"
#include "./Math/include/OPquat.h"
#include "./Core/include/OPmemory.h"
#include "./Core/include/OPmath.h"
#include "./Core/include/OPlog.h"

struct OPmat4;
inline void OPmat4Mul(OPmat4* dst, OPmat4 m1, OPmat4 m2);
inline void OPmat4RotX(OPmat4* m, OPfloat x);
inline void OPmat4RotY(OPmat4* m, OPfloat x);
inline void OPmat4RotZ(OPmat4* m, OPfloat x);
inline void OPmat4Translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4Scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline OPvec3 OPmat4Transform(OPvec3 a, OPmat4 b);
inline OPvec4 OPmat4Transform(OPvec4 a, OPmat4 b);

// Data Structure 4 * 4 = 16 floats
//		32 bit = 16 * 32 = 512 bits or 64 bytes
//		64 bit = 16 * 64 = 1024 bits or 128 bytes
struct OPmat4 {
	OPvec4 cols[4];

	OPmat4 operator=(OPmat4 vhs) { 
		OPmemcpy(this, (void*)&vhs, sizeof(OPmat4)); return *this;
	}
	inline OPvec4& operator[](const i32 i) {
		return cols[i];
	}
	inline OPmat4& operator*=(OPmat4 vhs) { 
		OPmat4Mul(this, *this, vhs); 
		return *this; 
	}
	inline OPmat4& operator*=(OPvec2 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, 1);
		return *this;
	}
	inline OPmat4& operator*=(OPvec3 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator*=(OPvec4 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator/=(OPvec2 vhs) {
		OPmat4Scl(this, vhs.x, vhs.y, 1);
		return *this;
	}
	inline OPmat4& operator/=(OPvec3 vhs) { 
		OPmat4Scl(this, vhs.x, vhs.y, vhs.z); 
		return *this; 
	}
	inline OPmat4& operator/=(OPvec4 vhs) { 
		OPmat4Scl(this, vhs.x, vhs.y, vhs.z); 
		return *this; 
	}
	inline OPmat4& operator+=(OPvec2 vhs) {
		OPmat4Translate(this, vhs.x, vhs.y, 0);
		return *this;
	}
	inline OPmat4& operator+=(OPvec2* vhs) {
		OPmat4Translate(this, vhs->x, vhs->y, 0);
		return *this;
	}
	inline OPmat4& operator+=(OPvec3 vhs) {
		OPmat4Translate(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec3* vhs) {
		OPmat4Translate(this, vhs->x, vhs->y, vhs->z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec4 vhs) {
		OPmat4Translate(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec4* vhs) {
		OPmat4Translate(this, vhs->x, vhs->y, vhs->z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec2 vhs) {
		OPmat4Translate(this, -vhs.x, -vhs.y, 0);
		return *this;
	}
	inline OPmat4& operator-=(OPvec2* vhs) {
		OPmat4Translate(this, -vhs->x, -vhs->y, 0);
		return *this;
	}
	inline OPmat4& operator-=(OPvec3 vhs) {
		OPmat4Translate(this, -vhs.x, -vhs.y, -vhs.z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec3* vhs) {
		OPmat4Translate(this, -vhs->x, -vhs->y, -vhs->z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec4 vhs) {
		OPmat4Translate(this, -vhs.x, -vhs.y, -vhs.z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec4* vhs) {
		OPmat4Translate(this, -vhs->x, -vhs->y, -vhs->z);
		return *this;
	}
};

inline OPvec3 operator*(OPvec3 lhs, OPmat4 rhs) { 
	return OPmat4Transform(lhs, rhs);
}

inline OPvec3 operator*(OPmat4 lhs, OPvec3 rhs) { 
	return OPmat4Transform(rhs, lhs);
}

inline OPmat4 operator*(OPmat4 lhs, OPmat4 rhs) {
	OPmat4 temp;
	OPmat4Mul(&temp, lhs, rhs);
	return temp;
}

//    ___            _         _ _        _       _ _   _              _   _    
//   / __|_  _ _ __ | |__  ___| (_)__    /_\  _ _(_) |_| |_  _ __  ___| |_(_)__ 
//   \__ \ || | '  \| '_ \/ _ \ | / _|  / _ \| '_| |  _| ' \| '  \/ -_)  _| / _|
//   |___/\_, |_|_|_|_.__/\___/_|_\__| /_/ \_\_| |_|\__|_||_|_|_|_\___|\__|_\__|
//        |__/                                                                  

extern const OPmat4 OPMAT4ZERO;
extern const OPmat4 OPMAT4IDENTITY; 

inline OPmat4 OPmat4Create(
	f32 _00, f32 _10, f32 _20, f32 _30,
	f32 _01, f32 _11, f32 _21, f32 _31,
	f32 _02, f32 _12, f32 _22, f32 _32,
	f32 _03, f32 _13, f32 _23, f32 _33) {

	OPmat4 temp = {
		_00, _10, _20, _30,
		_01, _11, _21, _31,
		_02, _12, _22, _32,
		_03, _13, _23, _33
	};

	return temp;
}

inline OPmat4 OPmat4Create(OPvec4 row0, OPvec4 row1, OPvec4 row2, OPvec4 row3) {
	return OPmat4Create(
		row0.x, row0.y, row0.z, row0.w,
		row1.x, row1.y, row1.z, row1.w,
		row2.x, row2.y, row2.z, row2.w,
		row3.x, row3.y, row3.z, row3.w
		);
}

// NOTE(garrett): pretty sure this shouldn't be inlined because of code bloat, look into this later.
inline void OPmat4Mul(OPmat4* dst, OPmat4 m1, OPmat4 m2)
{
	OPmat4 result;
	result[0][0] = m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3]; 
	result[1][0] = m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3]; 
	result[2][0] = m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3]; 
	result[3][0] = m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3]; 
	result[0][1] = m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3]; 
	result[1][1] = m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3]; 
	result[2][1] = m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3]; 
	result[3][1] = m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3]; 
	result[0][2] = m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3]; 
	result[1][2] = m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3]; 
	result[2][2] = m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3]; 
	result[3][2] = m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3]; 
	result[0][3] = m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3]; 
	result[1][3] = m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3]; 
	result[2][3] = m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3]; 
	result[3][3] = m1[0][3] * m2[3][0] + m1[1][3] * m2[3][1] + m1[2][3] * m2[3][2] + m1[3][3] * m2[3][3]; 
	*dst = result;
};

inline void OPmat4Identity(OPmat4* m) {
	OPbzero(m, sizeof(OPmat4));
	m->cols[0].x = 1;
	m->cols[1].y = 1;
	m->cols[2].z = 1;
	m->cols[3].w = 1;
}


 inline OPmat4 OPmat4Transpose(OPmat4 m){
	OPmat4 tmp;
	for(OPint i = 0; i < 4; i++) {
		for(OPint j = 0; j < 4; j++) {
			tmp[j][i] = m[i][j];
		}
	}
	return tmp;
}

 inline OPmat4 OPmat4RotX(OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4 m = OPMAT4IDENTITY;
	m[1][1] = t1;
	m[1][2] = t2;
	m[2][1] = -t2;
	m[2][2] = t1;
	return m;
}

 inline OPmat4 OPmat4RotY(OPfloat t){
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4 m = OPMAT4IDENTITY;
	m[0][0] = t1;
	m[0][2] = -t2;
	m[2][0] = t2;
	m[2][2] = t1;
	return m;
}

 inline OPmat4 OPmat4RotZ(OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4 m = OPMAT4IDENTITY;
	m[0][0] = t1;
	m[0][1] = t2;
	m[1][0] = -t2;
	m[1][1] = t1;
	return m;
}

 inline OPmat4 OPmat4Translate(OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4 m = OPMAT4IDENTITY;
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	return m;
}

 inline OPmat4 OPmat4Translate(OPvec3 v) {
	 OPmat4 m = OPMAT4IDENTITY;
	m[3].x = v.x;
	m[3].y = v.y;
	m[3].z = v.z;
	return m;
}

 inline OPmat4 OPmat4Scl(OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4 m = OPMAT4IDENTITY;
	m[0][0] = x;
	m[1][1] = y;
	m[2][2] = z;
	return m;
 }

 inline OPmat4 OPmat4RotX(OPmat4 m, OPfloat x) {
	OPmat4 result = OPMAT4ZERO;
	OPmat4 temp = OPmat4RotX(x);
	OPmat4Mul(&result, m, temp);
	return result;
 }

 inline OPmat4 OPmat4RotY(OPmat4 m, OPfloat x) {
	 OPmat4 result = OPMAT4ZERO;
	 OPmat4 temp = OPmat4RotY(x);
	 OPmat4Mul(&result, m, temp);
	 return result;
 }

 inline OPmat4 OPmat4RotZ(OPmat4 m, OPfloat x) {
	 OPmat4 result = OPMAT4ZERO;
	 OPmat4 temp = OPmat4RotZ(x);
	 OPmat4Mul(&result, m, temp);
	 return result;
 }

 inline OPmat4 OPmat4Scl(OPmat4 m, OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4 result = OPMAT4ZERO;
	 OPmat4 temp = OPmat4Scl(x, y, z);
	 OPmat4Mul(&result, m, temp);
	 return result;
 }

 inline OPmat4 OPmat4Translate(OPmat4 m, OPfloat x, OPfloat y, OPfloat z) {
	 m[3][0] += x;
	 m[3][1] += y;
	 m[3][2] += z;
	 return m;
}

 inline OPmat4 OPmat4Translate(OPmat4 m, OPvec3 v) {
	 m[3][0] += v.x;
	 m[3][1] += v.y;
	 m[3][2] += v.z;
	 return m;
}

 inline OPvec3 OPmat4Transform(OPvec3 a, OPmat4 b) {
	 OPvec3 result;
	 result.x = b[0].x * a.x + b[1].x * a.y + b[2].x * a.z;
	 result.y = b[0].y * a.x + b[1].y * a.y + b[2].y * a.z;
	 result.z = b[0].z * a.x + b[1].z * a.y + b[2].z * a.z;
	 return result;
 }

 inline OPvec4 OPmat4Transform(OPvec4 a, OPmat4 b) {
	 OPvec4 result;
	 result.x = b[0][0] * a.x + b[1][0] * a.y + b[2][0] * a.z + b[3][0] * a.w;
	 result.y = b[0][1] * a.x + b[1][1] * a.y + b[2][1] * a.z + b[3][1] * a.w;
	 result.z = b[0][2] * a.x + b[1][2] * a.y + b[2][2] * a.z + b[3][2] * a.w;
	 result.w = b[0][3] * a.x + b[1][3] * a.y + b[2][3] * a.z + b[3][3] * a.w;
	 return result;
 }

 inline void OPmat4RotX(OPmat4* m, OPfloat x) {
	 OPmat4 tmp = OPmat4RotX(x);
	 OPmat4Mul(m, *m, tmp);
 }

 inline void OPmat4RotY(OPmat4* m, OPfloat x) {
	 OPmat4 tmp = OPmat4RotY(x);
	 OPmat4Mul(m, *m, tmp);
 }

 inline void OPmat4RotZ(OPmat4* m, OPfloat x) {
	 OPmat4 tmp = OPmat4RotZ(x);
	 OPmat4Mul(m, *m, tmp);
 }

 inline void OPmat4Translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z){
	 m->cols[3].x += x;
	 m->cols[3].y += y;
	 m->cols[3].z += z;
 }

 inline void OPmat4Scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	 OPmat4 tmp = OPmat4Scl(x, y, z);
	 OPmat4Mul(m, *m, tmp);
 }

 //    ___             _   _               _     _       _ _   _              _   _    
 //   | __|  _ _ _  __| |_(_)___ _ _  __ _| |   /_\  _ _(_) |_| |_  _ __  ___| |_(_)__ 
 //   | _| || | ' \/ _|  _| / _ \ ' \/ _` | |  / _ \| '_| |  _| ' \| '  \/ -_)  _| / _|
 //   |_| \_,_|_||_\__|\__|_\___/_||_\__,_|_| /_/ \_\_| |_|\__|_||_|_|_|_\___|\__|_\__|
 //                                                                                    

/* NOTE(garrett): Pretty sure this unneeded now that we have OPquat
inline void OPmat4BuildQuat(OPmat4* dst, OPquat* qtr){
	f32 x = qtr->x;		f32 y = qtr->y;		f32 z = qtr->z;		f32 w = qtr->w;
	f32 x2 = x + x;		f32 y2 = y + y;		f32 z2 = z + z;
	f32 xx = x * x2;	f32 xy = x * y2;	f32 xz = x * z2;
	f32 yy = y * y2;	f32 yz = y * z2;	f32 zz = z * z2;
	f32 wx = w * x2;	f32 wy = w * y2;	f32 wz = w * z2;

	dst->cols[0].x = 1 - (yy + zz);
	dst->cols[0].y = xy - wz;
	dst->cols[0].z = xz + wy;

	dst->cols[1].x = xy + wz;
	dst->cols[1].y = 1 - (xx + zz);
	dst->cols[1].z = yz - wx;
	
	dst->cols[2].x = xz - wy;
	dst->cols[2].y = yz + wx;
	dst->cols[2].z = 1 - (xx + yy);

	dst->cols[3].x = 0;
	dst->cols[3].y = 0;
	dst->cols[3].z = 0;
	
	dst->cols[0].w = 0;
	dst->cols[1].w = 0;
	dst->cols[2].w = 0;
	dst->cols[3].w = 1;
}

inline void OPmat4quat(OPmat4* m, OPquat* qtr) {
	OPmat4 temp = OPMAT4ZERO;
	OPmat4BuildQuat(&temp, qtr);
	OPmat4Mul(m, m, &temp);
}*/

inline OPfloat OPmat4GetCofactor(OPfloat m0, OPfloat m1, OPfloat m2,
	                           OPfloat m3, OPfloat m4, OPfloat m5,
	                           OPfloat m6, OPfloat m7, OPfloat m8)
{
    return m0 * (m4 * m8 - m5 * m7) -
           m1 * (m3 * m8 - m5 * m6) +
           m2 * (m3 * m7 - m4 * m6);
}


inline void OPmat4Log(const OPchar* msg, OPmat4 m) {
	OPlog("%s:\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f",
		msg, 
		m[0][0], m[1][0], m[2][0], m[3][0], 
		m[0][1], m[1][1], m[2][1], m[3][1], 
		m[0][2], m[1][2], m[2][2], m[3][2], 
		m[0][3], m[1][3], m[2][3], m[3][3]);
}

inline OPmat4 OPmat4Read(OPstream* str) {
	OPmat4 temp = {
		OPvec4Read(str),
		OPvec4Read(str),
		OPvec4Read(str),
		OPvec4Read(str)
	};
	return temp;
}

inline void OPmat4Write(OPmat4 v, OPstream* str) {
	OPvec4Write(v[0], str);
	OPvec4Write(v[1], str);
	OPvec4Write(v[2], str);
	OPvec4Write(v[3], str);
}

OPmat4 OPmat4Ortho(OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar);
OPmat4 OPmat4LookAt(OPvec3 eye, OPvec3 at, OPvec3 up);
OPmat4 OPmat4Perspective(OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal);
OPint OPmat4Inverse(OPmat4* dst, OPmat4 a);

#endif