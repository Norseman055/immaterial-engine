#include "MathEngine.h"
#include <math.h>

Vect::Vect()
	: vx(0.0f), vy(0.0f), vz(0.0f), vw(1.0f)
{
};

Vect::Vect(const float inX, const float inY, const float inZ, const float inW)
	: vx(inX), vy(inY), vz(inZ), vw(inW)
{
};

Vect::Vect(const __m128 inM)
	: m(inM)
{
};

Vect::~Vect()
{
};

const void Vect::operator = (const Vect &t)
{
	this->vx = t.vx;
	this->vy = t.vy;
	this->vz = t.vz;
	this->vw = t.vw;
};

const Vect Vect::operator + (const Vect &t)const
{
	return Vect( vx+t.vx, vy+t.vy, vz+t.vz, 1.0f );
};

const void Vect::operator += (const Vect &t)
{
	this->vx += t.vx;
	this->vy += t.vy;
	this->vz += t.vz;
	this->vw = 1.0f;
};

const Vect Vect::operator - (const Vect &t)const
{
	return Vect ( vx-t.vx, vy-t.vy, vz-t.vz, 1.0f );
};

const void Vect::operator -= (const Vect &t)
{
	this->vx -= t.vx;
	this->vy -= t.vy;
	this->vz -= t.vz;
	this->vw = 1.0f;
};

const Vect Vect::operator * (const Matrix &M)const
{
	return Vect(((this->vx * M._m0) + (this->vy * M._m4) + (this->vz * M._m8) + (this->vw * M._m12)),
		((this->vx * M._m1) + (this->vy * M._m5) + (this->vz * M._m9) + (this->vw * M._m13)),
		((this->vx * M._m2) + (this->vy * M._m6) + (this->vz * M._m10) + (this->vw * M._m14)),
		((this->vx * M._m3) + (this->vy * M._m7) + (this->vz * M._m11) + (this->vw * M._m15)));
};

const void Vect::operator *= (const Matrix &M)
{
	float tmpx = this->vx;
	float tmpy = this->vy;
	float tmpz = this->vz;
	float tmpw = this->vw;

	this->vx = ((tmpx * M._m0) + (tmpy * M._m4) + (tmpz * M._m8) + (tmpw * M._m12));
	this->vy = ((tmpx * M._m1) + (tmpy * M._m5) + (tmpz * M._m9) + (tmpw * M._m13));
	this->vz = ((tmpx * M._m2) + (tmpy * M._m6) + (tmpz * M._m10) + (tmpw * M._m14));
	this->vw = ((tmpx * M._m3) + (tmpy * M._m7) + (tmpz * M._m11) + (tmpw * M._m15));
};


const Vect Vect::operator * (const float f)const
{
	return Vect(this->vx * f, this->vy * f, this->vz * f);
};

const void Vect::operator *= (const float f)
{
	this->vx *= f;
	this->vy *= f;
	this->vz *= f;
	this->vw = 1.0f;
};

const Vect operator * (const float f, const Vect &v)
{
	return Vect(v.vx * f, v.vy * f, v.vz * f);
};

const Vect operator + (const Vect &t)
{
	return Vect(t*1);
};

const Vect operator - (const Vect &t)
{
	return Vect(t*-1);
};

const float Vect::operator[](const x_enum)const
{
	return this->vx;
};

float& Vect::operator[](const x_enum)
{
	return this->vx;
};

const float Vect::operator[](const y_enum)const
{
	return this->vy;
};

float& Vect::operator[](const y_enum)
{
	return this->vy;
};

const float Vect::operator[](const z_enum)const
{
	return this->vz;
};

float& Vect::operator[](const z_enum)
{
	return this->vz;
};

const float Vect::operator[](const w_enum)const
{
	return this->vw;
};

float& Vect::operator[](const w_enum)
{
	return this->vw;
};

const bool Vect::isEqual(const Vect &t, const float &prec)const
{
	bool answer = false;
	if ((fabs(this->vx -t.vx) <= prec) && (fabs(this->vy -t.vy) <= prec) && (fabs(this->vz -t.vz) <= prec) && (fabs(this->vw -t.vw) <= prec))
		answer = true;

	return answer;
};

const bool Vect::isZero(const float &prec)const
{
	bool answer = false;
	if (fabs(this->vx) <= 0+prec && fabs(this->vy) <= 0+prec && fabs(this->vz) <= 0+prec && fabs(this->vw) <= 1+prec)
		answer = true;

	return answer;
};

const float Vect::dot(const Vect &t)const
{
	Vect dotted;
	dotted.vx = this->vx * t.vx;
	dotted.vy = this->vy * t.vy;
	dotted.vz = this->vz * t.vz;
	dotted.vw = 1;

	return (dotted.vx + dotted.vy + dotted.vz);
};

const Vect Vect::cross(const Vect &t)const
{
	return Vect((this->vy*t.vz - this->vz*t.vy), (this->vz*t.vx - this->vx*t.vz), (this->vx*t.vy - this->vy*t.vx), 1.0f);
};

const void Vect::norm()
{
	float magged = this->mag();
	this->vx /= magged;
	this->vy /= magged;
	this->vz /= magged;
};

const Vect Vect::getNorm()const
{
	Vect norming = Vect(this->m);
	norming.norm();

	return norming;
};

const float Vect::mag()const
{
	return sqrtf((this->vx * this->vx) + (this->vy * this->vy) + (this->vz * this->vz));
};

const float Vect::magSqr()const
{
	return ((this->vx * this->vx) + (this->vy * this->vy) + (this->vz * this->vz));
};

const float Vect::getAngle(const Vect &t)const
{
	// T = theta
	// cosT = (a.dot(b)/a.mag()*b.mag())
	// T = acosf(a.dot(b)/a.mag()*b.mag());

	return (acosf((this->dot(t)/(this->mag() * t.mag()))));
};

const void Vect::set(const float inx, const float iny, const float inz, const float inw)
{
	this->vx = inx;
	this->vy = iny;
	this->vz = inz;
	this->vw = inw;
};

const void Vect::set(const Vect &v)
{
	this->m = v.m;
};