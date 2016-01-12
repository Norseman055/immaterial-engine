/*****************************************************************************/
/*                                                                           */
/* File: Quat.cpp                                                            */
/*                                                                           */
/* Quaterion Class                                                           */
/*                                                                           */
/*****************************************************************************/

#include "MathEngine.h"
#include <math.h>
#include <assert.h>

Quat::Quat()
	: qx(0), qy(0), qz(0), qw(1)
{};

Quat::Quat(const Quat &q0)
	: qx(q0.qx), qy(q0.qy), qz(q0.qz), qw(q0.qw)
{};

Quat::Quat(const Matrix &M)
{
	float tr = M._m0 + M._m5 + M._m10;
	float S;

	if (tr > 0.0f)
	{
		S = 2 * sqrtf(tr + 1.0f);
		this->qw = 0.25f * S;
		this->qx = -(M._m9 - M._m6) / S;
		this->qy = -(M._m2 - M._m8) / S;
		this->qz = -(M._m4 - M._m1) / S;
	}
	else if ((M._m0 > M._m5) && (M._m0 > M._m10))
	{
		S = 2 * sqrtf(1.0f + M._m0 - M._m5 - M._m10);
		this->qw = -(M._m9 - M._m6) / S;
		this->qx = 0.25f * S;
		this->qy = (M._m1 + M._m4) / S;
		this->qz = (M._m2 + M._m8) / S;
	}
	else if (M._m5 > M._m10)
	{
		S = 2 * sqrtf(1.0f + M._m5 - M._m0 - M._m10);
		this->qw = -(M._m2 - M._m8) / S;
		this->qx = (M._m1 + M._m4) / S;
		this->qy = 0.25f * S;
		this->qz = (M._m6 + M._m9) / S;
	}
	else
	{
		S = 2 * sqrtf(1.0f + M._m10 - M._m0 - M._m5);
		this->qw = -(M._m4 - M._m1) / S;
		this->qx = (M._m2 + M._m8) / S;
		this->qy = (M._m6 + M._m9) / S;
		this->qz = 0.25f * S;
	}
};

Quat::Quat(const float f0, const float f1, const float f2, const float f3)
	: qx(f0), qy(f1), qz(f2), qw(f3)
{
};

Quat::Quat(const Vect &v, const float f0)
	: qx(v.vx), qy(v.vy), qz(v.vz), qw(f0)
{
};

Quat::Quat(const Rot3AxisType, const float f0, const float f1, const float f2)
{
	*this = Quat(Matrix(ROT_XYZ, f0, f1, f2));
};

Quat::Quat(const RotType in, const float f0)
{
	*this = Quat(Matrix(in, f0));
};

Quat::Quat(const RotAxisAngleType, const Vect &v, const float f0)
{
	*this = Quat(Matrix(ROT_AXIS_ANGLE, v, f0));
};

Quat::Quat(const RotOrientType in, const Vect &v0, const Vect &v1)
{
	*this = Quat(Matrix(in, v0, v1));
};

const float Quat::operator[](const x_enum)const
{
	return this->qx;
};

const float Quat::operator[](const y_enum)const
{
	return this->qy;
};

const float Quat::operator[](const z_enum)const
{
	return this->qz;
};

const float Quat::operator[](const w_enum)const
{
	return this->qw;
};

float& Quat::operator[](const x_enum)
{
	return this->qx;
};

float& Quat::operator[](const y_enum)
{
	return this->qy;
};

float& Quat::operator[](const z_enum)
{
	return this->qz;
};

float& Quat::operator[](const w_enum)
{
	return this->qw;
};

const void Quat::set(const float f0, const float f1, const float f2, const float f3)
{
	this->qx = f0;
	this->qy = f1;
	this->qz = f2;
	this->qw = f3;
};

const void Quat::set(const Matrix &_m)
{
	*this = Quat(_m);
};

const void Quat::set(const RotType in, const float f)
{
	switch (in)
	{
		case ROT_X:
			*this = Quat(Matrix(ROT_X, f));
			break;
		case ROT_Y:
			*this = Quat(Matrix(ROT_Y, f));
			break;
		case ROT_Z:
			*this = Quat(Matrix(ROT_Z, f));
			break;
	}
};

const void Quat::set(const Vect &v, const float f)
{
	this->qx = v.vx;
	this->qy = v.vy;
	this->qz = v.vz;
	this->qw = f;
};

const void Quat::set(const RotAxisAngleType, const Vect &v, const float f0)
{
	*this = Quat(Matrix(ROT_AXIS_ANGLE, v, f0));
};

const void Quat::set(const RotOrientType in, const Vect &v0, const Vect &v1)
{
	*this = Quat(Matrix(in, v0, v1));
};

const void Quat::set(const MatrixSpecialType in)
{
	switch(in)
	{
	case ZERO:
		*this = Quat(0, 0, 0, 0);
		break;
	case IDENTITY:
		*this = Quat(0, 0, 0, 1);
		break;
	}
};

const void Quat::set(const Rot3AxisType, const float f0, const float f1, const float f2)
{
	*this = Quat(Matrix(ROT_XYZ, f0, f1, f2));
};

const void Quat::setVect(const Vect &v0)
{
	this->qx = v0.vx;
	this->qy = v0.vy;
	this->qz = v0.vz;
};

const void Quat::getVect(Vect &v0)
{
	v0.vx = this->qx;
	v0.vy = this->qy;
	v0.vz = this->qz;
};

const void Quat::operator = (const Quat &q)
{
	this->qx = q.qx;
	this->qy = q.qy;
	this->qz = q.qz;
	this->qw = q.qw;
};

const Quat Quat::operator + (const Quat &q)const
{
	return Quat(this->qx + q.qx, this->qy + q.qy, this->qz + q.qz, this->qw + q.qw);
};

const Quat Quat::operator + (const float f0)const
{
	return Quat(this->qx + f0, this->qy + f0, this->qz + f0, this->qw + f0);
};

const void Quat::operator += (const Quat &q)
{
	this->qx += q.qx;
	this->qy += q.qy;
	this->qz += q.qz;
	this->qw += q.qw;
};

const void Quat::operator += (const float f0)
{
	this->qx += f0;
	this->qy += f0;
	this->qz += f0;
	this->qw += f0;
};

const Quat operator + (const float f0, const Quat &q)
{
	return Quat(q.qx + f0, q.qy + f0, q.qz + f0, q.qw + f0);
};

const Quat Quat::operator - (const Quat &q)const
{
	return Quat(this->qx - q.qx, this->qy - q.qy, this->qz - q.qz, this->qw - q.qw);
};

const Quat Quat::operator - (const float f0)const
{
	return Quat(this->qx - f0, this->qy - f0, this->qz - f0, this->qw - f0);
};

const void Quat::operator -= (const Quat &q)
{
	this->qx -= q.qx;
	this->qy -= q.qy;
	this->qz -= q.qz;
	this->qw -= q.qw;
};

const void Quat::operator -= (const float f0)
{
	this->qx -= f0;
	this->qy -= f0;
	this->qz -= f0;
	this->qw -= f0;
};

const Quat operator - (const float f0, const Quat &q)
{
	return Quat(f0 - q.qx, f0 - q.qy, f0 - q.qz, f0 - q.qw);
};

const Quat Quat::operator * (const Quat &q)const
{
	Vect vLeft (this->qx, this->qy, this->qz);
	Vect vRight (q.qx, q.qy, q.qz);
	return Quat(Vect(vRight.cross(vLeft) + (q.qw * vLeft) + (this->qw * vRight)), (this->qw * q.qw) - vLeft.dot(vRight));
};

const Quat Quat::operator * (const float f)const
{
	return Quat(this->qx * f, this->qy * f, this->qz * f, this->qw * f);
};

const void Quat::operator *= (const Quat &q)
{
	Vect vLeft (this->qx, this->qy, this->qz);
	Vect vRight (q.qx, q.qy, q.qz);
	*this = Quat(Vect(vRight.cross(vLeft) + (q.qw * vLeft) + (this->qw * vRight)), (this->qw * q.qw) - vLeft.dot(vRight));
};

const void Quat::operator *= (const float f)
{
	this->qx *= f;
	this->qy *= f;
	this->qz *= f;
	this->qw *= f;
};

const void Quat::operator *= (const Matrix &_m)
{
	*this = Quat(Matrix(*this) * _m);
};

const Quat operator * (const float f, const Quat &q)
{
	return Quat(q.qx * f, q.qy * f, q.qz * f, q.qw * f);
};

const Quat Quat::operator / (const Quat &q)const
{
	return Quat(this->qx / q.qx, this->qy / q.qy, this->qz / q.qz, this->qw / q.qw);
};

const Quat Quat::operator / (const float f0)const
{
	return Quat(this->qx / f0, this->qy / f0, this->qz / f0, this->qw / f0);
};

const void Quat::operator /= (const Quat &q)
{
	this->qx /= q.qx;
	this->qy /= q.qy;
	this->qz /= q.qz;
	this->qw /= q.qw;
};

const void Quat::operator /= (const float f0)
{
	this->qx /= f0;
	this->qy /= f0;
	this->qz /= f0;
	this->qw /= f0;
};

const Quat operator / (const float f0, const Quat &q)
{
	return Quat(f0 / q.qx, f0 / q.qy, f0 / q.qz, f0 / q.qw);
};

const Quat operator - (const Quat &q)
{
	return Quat(q*-1);
};

const Quat operator + (const Quat &q)
{
	return Quat(q);
};

const Quat Quat::multByElement(const Quat &q)
{
	return Quat(this->qx * q.qx, this->qy * q.qy, this->qz * q.qz, this->qw * q.qw);
};

const Quat Quat::getConj()const
{
	return Quat(this->qx * -1, this->qy * -1, this->qz * -1, this->qw);
};

const Quat Quat::getT()
{
	return Quat(Matrix(*this).getT());
};

const Quat Quat::norm()
{
	float mag = sqrtf(this->qx * this->qx + this->qy * this->qy + this->qz * this->qz + this->qw * this->qw);
	*this = Quat( this->qx / mag, this->qy / mag, this->qz / mag, this->qw / mag );
	return *this;
};

const Quat Quat::getNorm()const
{
	float mag = sqrtf(this->qx * this->qx + this->qy * this->qy + this->qz * this->qz + this->qw * this->qw);
	return Quat( this->qx / mag, this->qy / mag, this->qz / mag, this->qw / mag );
};

const Quat Quat::getInv()const
{
	return Quat(*this / this->magSquared()).getConj();
};

const void Quat::conj()
{
	this->qx *= -1;
	this->qy *= -1;
	this->qz *= -1;
};

const void Quat::T()
{
	*this = Quat(Matrix(*this).getT());
};

const void Quat::inv()
{
	*this = Quat(*this / this->magSquared()).getConj();
};

const void Quat::Lqcvq(const Vect &vIn, Vect &vOut)
{
	Vect tmp (this->qx, this->qy, this->qz);
	vOut = ((2 * this->qw) * tmp.cross(vIn)) + (((this->qw * this->qw) - tmp.dot(tmp)) * vIn) + 2 * (tmp.dot(vIn)) * tmp;
};

const void Quat::Lqvqc(const Vect &vIn, Vect &vOut)
{
	Vect tmp (this->qx, this->qy, this->qz);
	vOut = ((2 * this->qw) * vIn.cross(tmp)) + (((this->qw * this->qw) - tmp.dot(tmp)) * vIn) + 2 * (tmp.dot(vIn)) * tmp;
};

const void Quat::getAxis(Vect &v)
{
	v.vx = this->qx;
	v.vy = this->qy;
	v.vz = this->qz;
};

const float Quat::mag()const
{
	return sqrtf(this->qx * this->qx + this->qy * this->qy + this->qz * this->qz + this->qw * this->qw);
};

const float Quat::magSquared()const
{
	return (this->qx * this->qx + this->qy * this->qy + this->qz * this->qz + this->qw * this->qw);
};

const float Quat::invMag()const
{
	return 1 / this->mag();
};

const float Quat::dot(const Quat &q)const
{
	return this->qx * q.qx + this->qy * q.qy + this->qz * q.qz + this->qw * q.qw;
};

const float Quat::getAngle()const
{
	return 2 * (acosf(this->qw));
};

const bool Quat::isEqual(const Quat &q, const float f0)
{
	bool answer = false;
	if ((fabs(this->qx -q.qx) <= f0) && (fabs(this->qy -q.qy) <= f0) && (fabs(this->qz -q.qz) <= f0) && (fabs(this->qw -q.qw) <= f0))
		answer = true;

	return answer;
};

const bool Quat::isNegEqual(const Quat &q, const float f0)
{
	bool answer = false;
	if (((this->qx + q.qx) <= f0) && ((this->qy + q.qy) <= f0) && ((this->qz + q.qz) <= f0) && ((this->qw + q.qw) <= f0))
		answer = true;

	return answer;
};

const bool Quat::isEquivalent(const Quat &q, const float precision)
{
	bool answer = false;
	
	answer = this->isEqual(q, precision);

	if (!answer)
		answer = this->isEqual(-q, precision);

	return answer;
};

const bool Quat::isConjugateEqual(const Quat &q, const float precision)
{
	bool answer = false;
	if (this->qx + q.qx <= precision && this->qy + q.qy <= precision && this->qz + q.qz <= precision && this->qw - q.qw <= precision)
		answer = true;
	
	return answer;
};

const bool Quat::isIdentity(const float precision)
{
	bool answer = false;
	if ((fabs(this->qx) - precision <= 0) && (fabs(this->qy) - precision <= 0) && (fabs(this->qz) - precision <= 0) && (fabs(this->qw) - 1 - precision <= 0))
		answer = true;
	return answer;
};

const bool Quat::isNormalized(const float precision)
{
	bool answer = false;
	Vect tmp (this->qx, this->qy, this->qz);
	
	if ( fabs((this->qw * this->qw) + tmp.dot(tmp)) - 1 <= precision )
		answer = true;

	return answer;
};

const bool Quat::isZero(const float precision)
{
	bool answer = false;
	if (fabs(this->qx) <= precision && fabs(this->qy) <= precision && fabs(this->qz) <= precision && fabs(this->qw) <= precision)
		answer = true;
	return answer;
};

// do magic