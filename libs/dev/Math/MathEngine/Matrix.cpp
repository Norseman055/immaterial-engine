#include "MathEngine.h"
#include <math.h>

// Do your magic

Matrix::Matrix()
{
	this->v0 = Vect(0,0,0,0);
	this->v1 = Vect(0,0,0,0);
	this->v2 = Vect(0,0,0,0);
	this->v3 = Vect(0,0,0,0);
};

Matrix::Matrix(const Vect &in0, const Vect &in1, const Vect &in2, const Vect &in3)
	:v0(in0), v1(in1), v2(in2), v3(in3)
{
};

Matrix::Matrix(const float m0, const float m1, const float m2, const float m3,
		const float m4, const float m5, const float m6, const float m7,
		const float m8, const float m9, const float m10, const float m11,
		const float m12, const float m13, const float m14, const float m15)
{
	this->v0 = Vect(m0, m1, m2, m3);
	this->v1 = Vect(m4, m5, m6, m7);
	this->v2 = Vect(m8, m9, m10, m11);
	this->v3 = Vect(m12, m13, m14, m15);
};

Matrix::Matrix(const MatrixSpecialType in)
{
	switch (in)
	{	
		case ZERO:
			// Matrix of all 0's
			this->v0 = Vect(0,0,0,0);
			this->v1 = Vect(0,0,0,0);
			this->v2 = Vect(0,0,0,0);
			this->v3 = Vect(0,0,0,0);
			break;
		case IDENTITY:
			// Matrix of 1's along the diagonal
			this->v0 = Vect(1,0,0,0);
			this->v1 = Vect(0,1,0,0);
			this->v2 = Vect(0,0,1,0);
			this->v3 = Vect(0,0,0,1);
			break;
	}
};

Matrix::Matrix(const MatrixTransType, const float f0, const float f1, const float f2)
{
	this->v0 = Vect(1,0,0,0);
	this->v1 = Vect(0,1,0,0);
	this->v2 = Vect(0,0,1,0);
	this->v3 = Vect(f0,f1,f2,1);
};

Matrix::Matrix(const MatrixTransType, const Vect &v0)
{
	this->v0 = Vect(1,0,0,0);
	this->v1 = Vect(0,1,0,0);
	this->v2 = Vect(0,0,1,0);
	this->v3 = v0;
};

Matrix::Matrix(const MatrixScaleType, const float f0, const float f1, const float f2)
{
	this->v0 = Vect(f0,0,0,0);
	this->v1 = Vect(0,f1,0,0);
	this->v2 = Vect(0,0,f2,0);
	this->v3 = Vect(0,0,0,1);
};

Matrix::Matrix(const MatrixScaleType, const Vect &v0)
{
	this->v0 = Vect(v0.vx,0,0,0);
	this->v1 = Vect(0,v0.vy,0,0);
	this->v2 = Vect(0,0,v0.vz,0);
	this->v3 = Vect(0,0,0,1);
};

Matrix::Matrix(const RotType in, const float f)
{
	switch(in)
	{
		case ROT_X:
			this->v0 = Vect(1,0,0,0);
			this->v1 = Vect(0,cosf(f),sinf(f),0);
			this->v2 = Vect(0,-sinf(f),cosf(f),0);
			this->v3 = Vect(0,0,0,1);
			break;
		case ROT_Y:
			this->v0 = Vect(cosf(f),0,-sinf(f),0);
			this->v1 = Vect(0,1,0,0);
			this->v2 = Vect(sinf(f),0,cosf(f),0);
			this->v3 = Vect(0,0,0,1);
			break;
		case ROT_Z:
			this->v0 = Vect(cosf(f),sinf(f),0,0);
			this->v1 = Vect(-sinf(f),cosf(f),0,0);
			this->v2 = Vect(0,0,1,0);
			this->v3 = Vect(0,0,0,1);
			break;
	}
};

Matrix::Matrix(const RotAxisAngleType, const Vect &v0, const float f0)
{
	Vect tmp(v0);
	tmp.norm();

	this->_m0 = cosf(f0) + ((tmp[x] * tmp[x])*(1-cosf(f0)));
	this->_m1 = ((tmp[y]*tmp[x])*(1-cosf(f0))) + (tmp[z]*sinf(f0));
	this->_m2 = ((tmp[z]*tmp[x])*(1-cosf(f0))) - (tmp[y]*sinf(f0));
	this->_m3 = 0;
	this->_m4 = ((tmp[x]*tmp[y])*(1-cosf(f0))) - (tmp[z]*sinf(f0));
	this->_m5 = cosf(f0) + ((tmp[y]*tmp[y])*(1-cosf(f0)));
	this->_m6 = ((tmp[z]*tmp[y])*(1-cosf(f0))) + (tmp[x]*sinf(f0));
	this->_m7 = 0;
	this->_m8 = ((tmp[x]*tmp[z])*(1-cosf(f0))) + (tmp[y]*sinf(f0));
	this->_m9 = ((tmp[y]*tmp[z])*(1-cosf(f0))) - (tmp[x]*sinf(f0));
	this->_m10 = cosf(f0) +((tmp[z]*tmp[z])*(1-cosf(f0)));
	this->_m11 = 0;
	this->_m12 = 0;
	this->_m13 = 0;
	this->_m14 = 0;
	this->_m15 = 1;
};

Matrix::Matrix(const Rot3AxisType, const float f0, const float f1, const float f2)
{
	// this = mx * my * mz
	*this = Matrix(
			  Matrix(Vect(1,0,0,0), Vect(0,cosf(f0),sinf(f0),0), Vect(0,-sinf(f0),cosf(f0),0), Vect(0,0,0,1))
			* Matrix(Vect(cosf(f1),0,-sinf(f1),0), Vect(0,1,0,0), Vect(sinf(f1),0,cosf(f1),0), Vect(0,0,0,1))
			* Matrix(Vect(cosf(f2),sinf(f2),0,0), Vect(-sinf(f2),cosf(f2),0,0), Vect(0,0,1,0), Vect(0,0,0,1)) );
};

Matrix::Matrix(const RotOrientType in, const Vect &v0, const Vect &v1)
{
	// this solution for normal orientation was from work done by Ed Keenan in class.
	const Vect rz = v0.getNorm();

	Vect rx = v1.cross(rz);
	rx.norm();

	Vect ry = rz.cross(rx);
	ry.norm();
	
	this->set(rx, ry, rz, Vect(0.0f, 0.0f, 0.0f, 1.0f));
	this->_m3 = 0.0f;
	this->_m7 = 0.0f;
	this->_m11 = 0.0f;
	this->_m15 = 1.0f;

	switch (in)
	{
	case ROT_ORIENT:
		break;
	case ROT_INVERSE_ORIENT:
		this->T();
		break;
	}
};

Matrix::Matrix(const Matrix &m0)
	: v0(m0.v0), v1(m0.v1), v2(m0.v2), v3(m0.v3)
{
};

Matrix::Matrix(const Quat &q0)
{
	this->_m0 = 1 - (2 * (q0.qy * q0.qy + q0.qz * q0.qz));
	this->_m1 = 2 * (q0.qx * q0.qy + q0.qw * q0.qz);
	this->_m2 = 2 * (q0.qx * q0.qz - q0.qw * q0.qy);
	this->_m3 = 0;
	this->_m4 = 2 * (q0.qx * q0.qy - q0.qw * q0.qz);
	this->_m5 = 1 - (2 * (q0.qx * q0.qx + q0.qz * q0.qz));
	this->_m6 = 2 * (q0.qy * q0.qz + q0.qw * q0.qx);
	this->_m7 = 0;
	this->_m8 = 2 * (q0.qx * q0.qz + q0.qw * q0.qy);
	this->_m9 = 2 * (q0.qy * q0.qz - q0.qw * q0.qx);
	this->_m10 = 1 - (2 * (q0.qx * q0.qx + q0.qy * q0.qy));
	this->_m11 = 0;
	this->v3 = Vect(0, 0, 0, 1);
};

Matrix::~Matrix()
{
};

const void Matrix::set(const MatrixSpecialType in)
{
	*this = Matrix(in);
};

const void Matrix::set(const MatrixTransType, const float f0, const float f1, const float f2)
{
	*this = Matrix(TRANS, f0, f1, f2);
};

const void Matrix::set(const MatrixTransType, const Vect &_v0)
{
	*this = Matrix(TRANS, _v0.vx, _v0.vy, _v0.vz);
};

const void Matrix::set(const MatrixScaleType, const float f0, const float f1, const float f2)
{
	*this = Matrix(SCALE, f0, f1, f2);
};

const void Matrix::set(const MatrixScaleType, const Vect &_v0)
{
	*this = Matrix(SCALE, _v0.vx, _v0.vy, _v0.vz);
};

const void Matrix::set(const RotType in, const float f)
{
	switch (in)
	{
		case ROT_X:
			*this = Matrix(ROT_X, f);
			break;
		case ROT_Y:
			*this = Matrix(ROT_Y, f);
			break;
		case ROT_Z:
			*this = Matrix(ROT_Z, f);
			break;
	}
};

const void Matrix::set(const Rot3AxisType, const float f0, const float f1, const float f2)
{
	// this = mx * my * mz
	*this = Matrix(
			  Matrix(Vect(1,0,0,0), Vect(0,cosf(f0),sinf(f0),0), Vect(0,-sinf(f0),cosf(f0),0), Vect(0,0,0,1))
			* Matrix(Vect(cosf(f1),0,-sinf(f1),0), Vect(0,1,0,0), Vect(sinf(f1),0,cosf(f1),0), Vect(0,0,0,1))
			* Matrix(Vect(cosf(f2),sinf(f2),0,0), Vect(-sinf(f2),cosf(f2),0,0), Vect(0,0,1,0), Vect(0,0,0,1)) );
};

const void Matrix::set(const RotAxisAngleType, const Vect &_v0, const float f0)
{
	*this = Matrix(ROT_AXIS_ANGLE, _v0, f0);
};

const void Matrix::set(const RotOrientType in, const Vect &_v0, const Vect &_v1)
{
	*this = Matrix(in, _v0, _v1);
};

const void Matrix::set(const MatrixRowType in, const Vect &v)
{
	switch (in)
	{
		case ROW_0:
			this->v0 = v;
			break;
		case ROW_1:
			this->v1 = v;
			break;
		case ROW_2:
			this->v2 = v;
			break;
		case ROW_3:
			this->v3 = v;
			break;
	}
};

const void Matrix::set(const Vect &_v0, const Vect &_v1, const Vect &_v2, const Vect &_v3)
{
	*this = Matrix(_v0,_v1,_v2,_v3);
};

const void Matrix::set(const Quat &q0)
{
	*this = Matrix(q0);
};

const Vect Matrix::get(const MatrixRowType in)const
{
	Vect out;

	switch (in)
	{
		case ROW_0:
			out = this->v0;
			break;
		case ROW_1:
			out = this->v1;
			break;
		case ROW_2:
			out = this->v2;
			break;
		case ROW_3:
			out = this->v3;
			break;
	}

	return out;
};

const void Matrix::operator = (const Matrix &t)
{
	this->v0 = t.v0;
	this->v1 = t.v1;
	this->v2 = t.v2;
	this->v3 = t.v3;
};

const Matrix Matrix::operator + (const Matrix &t)const
{
	return Matrix(this->_m0 + t._m0, this->_m1 + t._m1, this->_m2 + t._m2, this->_m3 + t._m3,
				  this->_m4 + t._m4, this->_m5 + t._m5, this->_m6 + t._m6, this->_m7 + t._m7,
				  this->_m8 + t._m8, this->_m9 + t._m9, this->_m10 + t._m10, this->_m11 + t._m11,
		  		  this->_m12 + t._m12, this->_m13 + t._m13, this->_m14 + t._m14, this->_m15 + t._m15);
};

const void Matrix::operator += (const Matrix &t)
{
	this->_m0 += t._m0;
	this->_m1 += t._m1;
	this->_m2 += t._m2;
	this->_m3 += t._m3;
	this->_m4 += t._m4;
	this->_m5 += t._m5;
	this->_m6 += t._m6;
	this->_m7 += t._m7;
	this->_m8 += t._m8;
	this->_m9 += t._m9;
	this->_m10 += t._m10;
	this->_m11 += t._m11;
	this->_m12 += t._m12;
	this->_m13 += t._m13;
	this->_m14 += t._m14;
	this->_m15 += t._m15;
};

const Matrix Matrix::operator - (const Matrix &t)const
{
	return Matrix(this->_m0 - t._m0, this->_m1 - t._m1, this->_m2 - t._m2, this->_m3 - t._m3,
		this->_m4 - t._m4, this->_m5 - t._m5, this->_m6 - t._m6, this->_m7 - t._m7,
		this->_m8 - t._m8, this->_m9 - t._m9, this->_m10 - t._m10, this->_m11 - t._m11,
		this->_m12 - t._m12, this->_m13 - t._m13, this->_m14 - t._m14, this->_m15 - t._m15);
};

const void Matrix::operator -= (const Matrix &t)
{
	this->_m0 -= t._m0;
	this->_m1 -= t._m1;
	this->_m2 -= t._m2;
	this->_m3 -= t._m3;
	this->_m4 -= t._m4;
	this->_m5 -= t._m5;
	this->_m6 -= t._m6;
	this->_m7 -= t._m7;
	this->_m8 -= t._m8;
	this->_m9 -= t._m9;
	this->_m10 -= t._m10;
	this->_m11 -= t._m11;
	this->_m12 -= t._m12;
	this->_m13 -= t._m13;
	this->_m14 -= t._m14;
	this->_m15 -= t._m15;
};

const Matrix Matrix::operator * (const Matrix &t)const
{
	Matrix ans;

	ans._m0 = ((this->_m0 * t._m0) + (this->_m1 * t._m4) + (this->_m2 * t._m8) + (this->_m3 * t._m12));
	ans._m1 = ((this->_m0 * t._m1) + (this->_m1 * t._m5) + (this->_m2 * t._m9) + (this->_m3 * t._m13));
	ans._m2 = ((this->_m0 * t._m2) + (this->_m1 * t._m6) + (this->_m2 * t._m10) + (this->_m3 * t._m14));
	ans._m3 = ((this->_m0 * t._m3) + (this->_m1 * t._m7) + (this->_m2 * t._m11) + (this->_m3 * t._m15));
	ans._m4 = ((this->_m4 * t._m0) + (this->_m5 * t._m4) + (this->_m6 * t._m8) + (this->_m7 * t._m12));
	ans._m5 = ((this->_m4 * t._m1) + (this->_m5 * t._m5) + (this->_m6 * t._m9) + (this->_m7 * t._m13));
	ans._m6 = ((this->_m4 * t._m2) + (this->_m5 * t._m6) + (this->_m6 * t._m10) + (this->_m7 * t._m14));
	ans._m7 = ((this->_m4 * t._m3) + (this->_m5 * t._m7) + (this->_m6 * t._m11) + (this->_m7 * t._m15));
	ans._m8 = ((this->_m8 * t._m0) + (this->_m9 * t._m4) + (this->_m10 * t._m8) + (this->_m11 * t._m12));
	ans._m9 = ((this->_m8 * t._m1) + (this->_m9 * t._m5) + (this->_m10 * t._m9) + (this->_m11 * t._m13));
	ans._m10 = ((this->_m8 * t._m2) + (this->_m9 * t._m6) + (this->_m10 * t._m10) + (this->_m11 * t._m14));
	ans._m11 = ((this->_m8 * t._m3) + (this->_m9 * t._m7) + (this->_m10 * t._m11) + (this->_m11 * t._m15));
	ans._m12 = ((this->_m12 * t._m0) + (this->_m13 * t._m4) + (this->_m14 * t._m8) + (this->_m15 * t._m12));
	ans._m13 = ((this->_m12 * t._m1) + (this->_m13 * t._m5) + (this->_m14 * t._m9) + (this->_m15 * t._m13));
	ans._m14 = ((this->_m12 * t._m2) + (this->_m13 * t._m6) + (this->_m14 * t._m10) + (this->_m15 * t._m14));
	ans._m15 = ((this->_m12 * t._m3) + (this->_m13 * t._m7) + (this->_m14 * t._m11) + (this->_m15 * t._m15));

	return ans;
};

const void Matrix::operator *= (const Matrix &t)
{
	Matrix tmp = Matrix(*this);

	this->_m0 = ((tmp._m0 * t._m0) + (tmp._m1 * t._m4) + (tmp._m2 * t._m8) + (tmp._m3 * t._m12));
	this->_m1 = ((tmp._m0 * t._m1) + (tmp._m1 * t._m5) + (tmp._m2 * t._m9) + (tmp._m3 * t._m13));
	this->_m2 = ((tmp._m0 * t._m2) + (tmp._m1 * t._m6) + (tmp._m2 * t._m10) + (tmp._m3 * t._m14));
	this->_m3 = ((tmp._m0 * t._m3) + (tmp._m1 * t._m7) + (tmp._m2 * t._m11) + (tmp._m3 * t._m15));
	this->_m4 = ((tmp._m4 * t._m0) + (tmp._m5 * t._m4) + (tmp._m6 * t._m8) + (tmp._m7 * t._m12));
	this->_m5 = ((tmp._m4 * t._m1) + (tmp._m5 * t._m5) + (tmp._m6 * t._m9) + (tmp._m7 * t._m13));
	this->_m6 = ((tmp._m4 * t._m2) + (tmp._m5 * t._m6) + (tmp._m6 * t._m10) + (tmp._m7 * t._m14));
	this->_m7 = ((tmp._m4 * t._m3) + (tmp._m5 * t._m7) + (tmp._m6 * t._m11) + (tmp._m7 * t._m15));
	this->_m8 = ((tmp._m8 * t._m0) + (tmp._m9 * t._m4) + (tmp._m10 * t._m8) + (tmp._m11 * t._m12));
	this->_m9 = ((tmp._m8 * t._m1) + (tmp._m9 * t._m5) + (tmp._m10 * t._m9) + (tmp._m11 * t._m13));
	this->_m10 = ((tmp._m8 * t._m2) + (tmp._m9 * t._m6) + (tmp._m10 * t._m10) + (tmp._m11 * t._m14));
	this->_m11 = ((tmp._m8 * t._m3) + (tmp._m9 * t._m7) + (tmp._m10 * t._m11) + (tmp._m11 * t._m15));
	this->_m12 = ((tmp._m12 * t._m0) + (tmp._m13 * t._m4) + (tmp._m14 * t._m8) + (tmp._m15 * t._m12));
	this->_m13 = ((tmp._m12 * t._m1) + (tmp._m13 * t._m5) + (tmp._m14 * t._m9) + (tmp._m15 * t._m13));
	this->_m14 = ((tmp._m12 * t._m2) + (tmp._m13 * t._m6) + (tmp._m14 * t._m10) + (tmp._m15 * t._m14));
	this->_m15 = ((tmp._m12 * t._m3) + (tmp._m13 * t._m7) + (tmp._m14 * t._m11) + (tmp._m15 * t._m15));
};

const void Matrix::operator *= (const float f)
{
	this->_m0 *= f;
	this->_m1 *= f;
	this->_m2 *= f;
	this->_m3 *= f;
	this->_m4 *= f;
	this->_m5 *= f;
	this->_m6 *= f;
	this->_m7 *= f;
	this->_m8 *= f;
	this->_m9 *= f;
	this->_m10 *= f;
	this->_m11 *= f;
	this->_m12 *= f;
	this->_m13 *= f;
	this->_m14 *= f;
	this->_m15 *= f;
};

const Matrix operator * (const float f, const Matrix &t)
{
	return Matrix(t._m0 * f, t._m1 * f, t._m2 * f, t._m3 * f,
		t._m4 * f, t._m5 * f, t._m6 * f, t._m7 * f,
		t._m8 * f, t._m9 * f, t._m10 * f, t._m11 * f,
		t._m12 * f, t._m13 * f, t._m14 * f, t._m15 * f);
};

const Matrix operator * (const Matrix &t, const float f)
{
	return Matrix(t._m0 * f, t._m1 * f, t._m2 * f, t._m3 * f,
		t._m4 * f, t._m5 * f, t._m6 * f, t._m7 * f,
		t._m8 * f, t._m9 * f, t._m10 * f, t._m11 * f,
		t._m12 * f, t._m13 * f, t._m14 * f, t._m15 * f);
};

const Matrix operator + (const Matrix &t)
{
	return Matrix(t);
};

const Matrix operator - (const Matrix &t)
{
	return Matrix(t*-1);
};

const float Matrix::operator[](const m0_enum)const
{
	return this->_m0;
};
const float Matrix::operator[](const m1_enum)const
{
	return this->_m1;
};
const float Matrix::operator[](const m2_enum)const
{
	return this->_m2;
};
const float Matrix::operator[](const m3_enum)const
{
	return this->_m3;
};
const float Matrix::operator[](const m4_enum)const
{
	return this->_m4;
};
const float Matrix::operator[](const m5_enum)const
{
	return this->_m5;
};
const float Matrix::operator[](const m6_enum)const
{
	return this->_m6;
};
const float Matrix::operator[](const m7_enum)const
{
	return this->_m7;
};
const float Matrix::operator[](const m8_enum)const
{
	return this->_m8;
};
const float Matrix::operator[](const m9_enum)const
{
	return this->_m9;
};
const float Matrix::operator[](const m10_enum)const
{
	return this->_m10;
};
const float Matrix::operator[](const m11_enum)const
{
	return this->_m11;
};
const float Matrix::operator[](const m12_enum)const
{
	return this->_m12;
};
const float Matrix::operator[](const m13_enum)const
{
	return this->_m13;
};
const float Matrix::operator[](const m14_enum)const
{
	return this->_m14;
};
const float Matrix::operator[](const m15_enum)const
{
	return this->_m15;
};

float& Matrix::operator[](const m0_enum)
{
	return this->_m0;
};
float& Matrix::operator[](const m1_enum)
{
	return this->_m1;
};
float& Matrix::operator[](const m2_enum)
{
	return this->_m2;
};
float& Matrix::operator[](const m3_enum)
{
	return this->_m3;
};
float& Matrix::operator[](const m4_enum)
{
	return this->_m4;
};
float& Matrix::operator[](const m5_enum)
{
	return this->_m5;
};
float& Matrix::operator[](const m6_enum)
{
	return this->_m6;
};
float& Matrix::operator[](const m7_enum)
{
	return this->_m7;
};
float& Matrix::operator[](const m8_enum)
{
	return this->_m8;
};
float& Matrix::operator[](const m9_enum)
{
	return this->_m9;
};
float& Matrix::operator[](const m10_enum)
{
	return this->_m10;
};
float& Matrix::operator[](const m11_enum)
{
	return this->_m11;
};
float& Matrix::operator[](const m12_enum)
{
	return this->_m12;
};
float& Matrix::operator[](const m13_enum)
{
	return this->_m13;
};
float& Matrix::operator[](const m14_enum)
{
	return this->_m14;
};
float& Matrix::operator[](const m15_enum)
{
	return this->_m15;
};

const bool Matrix::isEqual(const Matrix &t)const
{
	bool equal = false;
	if (this->v0.isEqual(t.v0) && this->v1.isEqual(t.v1) && this->v2.isEqual(t.v2) && this->v3.isEqual(t.v3))
		equal = true;

	return equal;
};

const bool Matrix::isIdentity(const float f)const
{
	bool equal = false;
	if (this->v0.isEqual(Vect(1,0,0,0), f) && this->v1.isEqual(Vect(0,1,0,0), f) && this->v2.isEqual(Vect(0,0,1,0), f) && this->v3.isEqual(Vect(0,0,0,1), f))
		equal = true;

	return equal;
};

const float Matrix::det()const
{
	return 
		 (this->_m0 * (this->_m5 * (this->_m10 * this->_m15 - this->_m11 * this->_m14) - this->_m6 * (this->_m9 * this->_m15 - this->_m11 * this->_m13) + this->_m7 * (this->_m9 * this->_m14 - this->_m10 * this->_m13)))
		- (this->_m1 * (this->_m4 * (this->_m10 * this->_m15 - this->_m11 * this->_m14) - this->_m6 * (this->_m8 * this->_m15 - this->_m11 * this->_m12) + this->_m7 * (this->_m8 * this->_m14 - this->_m10 * this->_m12)))
		+ (this->_m2 * (this->_m4 * (this->_m9 * this->_m15 - this->_m11 * this->_m13) - this->_m5 * (this->_m8 * this->_m15 - this->_m11 * this->_m12) + this->_m7 * (this->_m8 * this->_m13 - this->_m9 * this->_m12)))
		- (this->_m3 * (this->_m4 * (this->_m9 * this->_m14 - this->_m10 * this->_m13) - this->_m5 * (this->_m8 * this->_m14 - this->_m10 * this->_m12) + this->_m6 * (this->_m8 * this->_m13 - this->_m9 * this->_m12)));
};

const void Matrix::T()
{
	*this = Matrix(this->_m0, this->_m4, this->_m8, this->_m12,
				   this->_m1, this->_m5, this->_m9, this->_m13,
				   this->_m2, this->_m6, this->_m10, this->_m14,
				   this->_m3, this->_m7, this->_m11, this->_m15);
};

const Matrix Matrix::getT()const
{
	return Matrix(this->_m0, this->_m4, this->_m8, this->_m12,
				  this->_m1, this->_m5, this->_m9, this->_m13,
				  this->_m2, this->_m6, this->_m10, this->_m14,
				  this->_m3, this->_m7, this->_m11, this->_m15);
};

const void Matrix::inv()
{
	Matrix tmp;
	tmp._m0 = ((_m6 * _m11 * _m13) - (_m7 * _m10 * _m13) + (_m7 * _m9 * _m14) - (_m5 * _m11 * _m14) - (_m6 * _m9 * _m15) + (_m5 * _m10 * _m15));
	tmp._m1 = ((_m3 * _m10 * _m13) - (_m2 * _m11 * _m13) - (_m3 * _m9 * _m14) + (_m1 * _m11 * _m14) + (_m2 * _m9 * _m15) - (_m1 * _m10 * _m15));
	tmp._m2 = ((_m2 * _m7 * _m13) - (_m3 * _m6 * _m13) + (_m3 * _m5 * _m14) - (_m1 * _m7 * _m14) - (_m2 * _m5 * _m15) + (_m1 * _m6 * _m15));
	tmp._m3 = ((_m3 * _m6 * _m9) - (_m2 * _m7 * _m9) - (_m3 * _m5 * _m10) + (_m1 * _m7 * _m10) + (_m2 * _m5 * _m11) - (_m1 * _m6 * _m11));
	tmp._m4 = ((_m7 * _m10 * _m12) - (_m6 * _m11 * _m12) - (_m7 * _m8 * _m14) + (_m4 * _m11 * _m14) + (_m6 * _m8 * _m15) - (_m4 * _m10 * _m15));
	tmp._m5 = ((_m2 * _m11 * _m12) - (_m3 * _m10 * _m12) + (_m3 * _m8 * _m14) - (_m0 * _m11 * _m14) - (_m2 * _m8 * _m15) + (_m0 * _m10 * _m15));
	tmp._m6 = ((_m3 * _m6 * _m12) - (_m2 * _m7 * _m12) - (_m3 * _m4 * _m14) + (_m0 * _m7 * _m14) + (_m2 * _m4 * _m15) - (_m0 * _m6 * _m15));
	tmp._m7 = ((_m2 * _m7 * _m8) - (_m3 * _m6 * _m8) + (_m3 * _m4 * _m10) - (_m0 * _m7 * _m10) - (_m2 * _m4 * _m11) + (_m0 * _m6 * _m11));
	tmp._m8 = ((_m5 * _m11 * _m12) - (_m7 * _m9 * _m12) + (_m7 * _m8 * _m13) - (_m4 * _m11 * _m13) - (_m5 * _m8 * _m15) + (_m4 * _m9 * _m15));
	tmp._m9 = ((_m3 * _m9 * _m12) - (_m1 * _m11 * _m12) - (_m3 * _m8 * _m13) + (_m0 * _m11 * _m13) + (_m1 * _m8 * _m15) - (_m0 * _m9 * _m15));
	tmp._m10 = ((_m1 * _m7 * _m12) - (_m3 * _m5 * _m12) + (_m3 * _m4 * _m13) - (_m0 * _m7 * _m13) - (_m1 * _m4 * _m15) + (_m0 * _m5 * _m15));
	tmp._m11 = ((_m3 * _m5 * _m8) - (_m1 * _m7 * _m8) - (_m3 * _m4 * _m9) + (_m0 * _m7 * _m9) + (_m1 * _m4 * _m11) - (_m0 * _m5 * _m11));
	tmp._m12 = ((_m6 * _m9 * _m12) - (_m5 * _m10 * _m12) - (_m6 * _m8 * _m13) + (_m4 * _m10 * _m13) + (_m5 * _m8 * _m14) - (_m4 * _m9 * _m14));
	tmp._m13 = ((_m1 * _m10 * _m12) - (_m2 * _m9 * _m12) + (_m2 * _m8 * _m13) - (_m0 * _m10 * _m13) - (_m1 * _m8 * _m14) + (_m0 * _m9 * _m14));
	tmp._m14 = ((_m2 * _m5 * _m12) - (_m1 * _m6 * _m12) - (_m2 * _m4 * _m13) + (_m0 * _m6 * _m13) + (_m1 * _m4 * _m14) - (_m0 * _m5 * _m14));
	tmp._m15 = ((_m1 * _m6 * _m8) - (_m2 * _m5 * _m8) + (_m2 * _m4 * _m9) - (_m0 * _m6 * _m9) - (_m1 * _m4 * _m10) + (_m0 * _m5 * _m10));

	*this = tmp;
};

const Matrix Matrix::getInv()const
{
	Matrix tmp;
	Matrix tmp2 = *this;

	tmp._m0 = ((_m6 * _m11 * _m13) - (_m7 * _m10 * _m13) + (_m7 * _m9 * _m14) - (_m5 * _m11 * _m14) - (_m6 * _m9 * _m15) + (_m5 * _m10 * _m15));
	tmp._m1 = ((_m3 * _m10 * _m13) - (_m2 * _m11 * _m13) - (_m3 * _m9 * _m14) + (_m1 * _m11 * _m14) + (_m2 * _m9 * _m15) - (_m1 * _m10 * _m15));
	tmp._m2 = ((_m2 * _m7 * _m13) - (_m3 * _m6 * _m13) + (_m3 * _m5 * _m14) - (_m1 * _m7 * _m14) - (_m2 * _m5 * _m15) + (_m1 * _m6 * _m15));
	tmp._m3 = ((_m3 * _m6 * _m9) - (_m2 * _m7 * _m9) - (_m3 * _m5 * _m10) + (_m1 * _m7 * _m10) + (_m2 * _m5 * _m11) - (_m1 * _m6 * _m11));
	tmp._m4 = ((_m7 * _m10 * _m12) - (_m6 * _m11 * _m12) - (_m7 * _m8 * _m14) + (_m4 * _m11 * _m14) + (_m6 * _m8 * _m15) - (_m4 * _m10 * _m15));
	tmp._m5 = ((_m2 * _m11 * _m12) - (_m3 * _m10 * _m12) + (_m3 * _m8 * _m14) - (_m0 * _m11 * _m14) - (_m2 * _m8 * _m15) + (_m0 * _m10 * _m15));
	tmp._m6 = ((_m3 * _m6 * _m12) - (_m2 * _m7 * _m12) - (_m3 * _m4 * _m14) + (_m0 * _m7 * _m14) + (_m2 * _m4 * _m15) - (_m0 * _m6 * _m15));
	tmp._m7 = ((_m2 * _m7 * _m8) - (_m3 * _m6 * _m8) + (_m3 * _m4 * _m10) - (_m0 * _m7 * _m10) - (_m2 * _m4 * _m11) + (_m0 * _m6 * _m11));
	tmp._m8 = ((_m5 * _m11 * _m12) - (_m7 * _m9 * _m12) + (_m7 * _m8 * _m13) - (_m4 * _m11 * _m13) - (_m5 * _m8 * _m15) + (_m4 * _m9 * _m15));
	tmp._m9 = ((_m3 * _m9 * _m12) - (_m1 * _m11 * _m12) - (_m3 * _m8 * _m13) + (_m0 * _m11 * _m13) + (_m1 * _m8 * _m15) - (_m0 * _m9 * _m15));
	tmp._m10 = ((_m1 * _m7 * _m12) - (_m3 * _m5 * _m12) + (_m3 * _m4 * _m13) - (_m0 * _m7 * _m13) - (_m1 * _m4 * _m15) + (_m0 * _m5 * _m15));
	tmp._m11 = ((_m3 * _m5 * _m8) - (_m1 * _m7 * _m8) - (_m3 * _m4 * _m9) + (_m0 * _m7 * _m9) + (_m1 * _m4 * _m11) - (_m0 * _m5 * _m11));
	tmp._m12 = ((_m6 * _m9 * _m12) - (_m5 * _m10 * _m12) - (_m6 * _m8 * _m13) + (_m4 * _m10 * _m13) + (_m5 * _m8 * _m14) - (_m4 * _m9 * _m14));
	tmp._m13 = ((_m1 * _m10 * _m12) - (_m2 * _m9 * _m12) + (_m2 * _m8 * _m13) - (_m0 * _m10 * _m13) - (_m1 * _m8 * _m14) + (_m0 * _m9 * _m14));
	tmp._m14 = ((_m2 * _m5 * _m12) - (_m1 * _m6 * _m12) - (_m2 * _m4 * _m13) + (_m0 * _m6 * _m13) + (_m1 * _m4 * _m14) - (_m0 * _m5 * _m14));
	tmp._m15 = ((_m1 * _m6 * _m8) - (_m2 * _m5 * _m8) + (_m2 * _m4 * _m9) - (_m0 * _m6 * _m9) - (_m1 * _m4 * _m10) + (_m0 * _m5 * _m10));

	return ((1/tmp2.det())*tmp);
};
