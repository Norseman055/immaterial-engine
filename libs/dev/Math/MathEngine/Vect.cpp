#include "MathEngine.h"
#include <math.h>

Vect::Vect()
	: m( _mm_setr_ps( 0, 0, 0, 1 ) ) { }

Vect::Vect( const float inX, const float inY, const float inZ, const float inW )
	: m( _mm_setr_ps( inX, inY, inZ, inW ) ) { }

Vect::Vect( const __m128& inM )
	: m( inM ) { }

Vect::~Vect() { }

void Vect::operator=(const Vect &t) {
	this->m = t.m;
};

Vect Vect::operator+(const Vect &t)const {
	return Vect( this->vx + t.vx, this->vy + t.vy, this->vz + t.vz );
};

void Vect::operator+=(const Vect &t) {
	_mm_store_ps( reinterpret_cast< float* >(&this->m), _mm_add_ps( this->m, t.m ) );
	this->vw = 1.0f;
};

Vect Vect::operator-(const Vect &t)const {
	return Vect( this->vx - t.vx, this->vy - t.vy, this->vz - t.vz );
};

void Vect::operator-=(const Vect &t) {
	_mm_store_ps( reinterpret_cast< float* >(&this->m), _mm_sub_ps( this->m, t.m ) );
	this->vw = 1.0f;
};

Vect Vect::operator*(const Matrix &M)const {
	return Vect( _mm_add_ps(
		_mm_add_ps( _mm_mul_ps( _mm_set_ps1( this->vx ), M.xm0 ), _mm_mul_ps( _mm_set_ps1( this->vy ), M.xm1 ) ),
		_mm_add_ps( _mm_mul_ps( _mm_set_ps1( this->vz ), M.xm2 ), _mm_mul_ps( _mm_set_ps1( this->vw ), M.xm3 ) ) )
		);
};

void Vect::operator*=(const Matrix &M) {
	auto tmp = *this;
	_mm_store_ps( reinterpret_cast< float* >(&this->m), _mm_add_ps(
		_mm_add_ps( _mm_mul_ps( _mm_set_ps1( this->vx ), M.xm0 ), _mm_mul_ps( _mm_set_ps1( this->vy ), M.xm1 ) ),
		_mm_add_ps( _mm_mul_ps( _mm_set_ps1( this->vz ), M.xm2 ), _mm_mul_ps( _mm_set_ps1( this->vw ), M.xm3 ) ) )
		);
};

Vect Vect::operator*(const float f)const {
	return Vect( this->vx * f, this->vy * f, this->vz * f );
};

void Vect::operator*=(const float f) {
	_mm_store_ps( reinterpret_cast< float* >(&this->m), _mm_mul_ps( this->m, _mm_set_ps1( f ) ) );
	this->vw = 1.0f;
};

Vect operator*(const float f, const Vect &v) {
	return Vect( v.vx * f, v.vy * f, v.vz * f );
};

Vect operator+(const Vect &t) {
	return Vect( t.vx, t.vy, t.vz );
};

Vect operator-(const Vect &t) {
	return Vect( t * -1 );
};

float Vect::operator[]( const x_enum )const {
	return this->vx;
};

float& Vect::operator[]( const x_enum ) {
	return this->vx;
};

float Vect::operator[]( const y_enum )const {
	return this->vy;
};

float& Vect::operator[]( const y_enum ) {
	return this->vy;
};

float Vect::operator[]( const z_enum )const {
	return this->vz;
};

float& Vect::operator[]( const z_enum ) {
	return this->vz;
};

float Vect::operator[]( const w_enum )const {
	return this->vw;
};

float& Vect::operator[]( const w_enum ) {
	return this->vw;
};

bool Vect::isEqual( const Vect &t, const float &prec )const {
	auto answer = false;
	if ( fabs( this->vx - t.vx ) <= prec && fabs( this->vy - t.vy ) <= prec && fabs( this->vz - t.vz ) <= prec && fabs( this->vw - t.vw ) <= prec ) {
		answer = true;
	}
	return answer;
};

bool Vect::isZero( const float &prec )const {
	auto answer = false;
	if ( fabs( this->vx ) <= 0 + prec && fabs( this->vy ) <= 0 + prec && fabs( this->vz ) <= 0 + prec && fabs( this->vw ) <= 1 + prec ) {
		answer = true;
	}
	return answer;
};

float Vect::dot( const Vect &t )const {
	Vect dotted( _mm_mul_ps( this->m, t.m ) );
	return (dotted.vx + dotted.vy + dotted.vz);
};

Vect Vect::cross( const Vect &t )const {
	return Vect( this->vy * t.vz - this->vz * t.vy, this->vz * t.vx - this->vx * t.vz, this->vx * t.vy - this->vy * t.vx, this->vw );
};

void Vect::norm() {
	_mm_store_ps( reinterpret_cast< float* >(&this->m), _mm_div_ps( this->m, _mm_set_ps1( this->mag() ) ) );
	this->vw = 1.0f;
};

Vect Vect::getNorm()const {
	Vect norming( this->m );
	norming.norm();

	return norming;
};

float Vect::mag()const {
	return sqrtf( this->magSqr() );
};

float Vect::magSqr()const {
	return this->vx * this->vx + this->vy * this->vy + this->vz * this->vz;
};

float Vect::getAngle( const Vect &t )const {
	// T = theta
	// cosT = (a.dot(b)/a.mag()*b.mag())
	// T = acosf(a.dot(b)/a.mag()*b.mag());

	return acosf( this->dot( t ) / (this->mag() * t.mag()) );
};

void Vect::set( const float inx, const float iny, const float inz, const float inw ) {
	_mm_store_ps( reinterpret_cast< float* >(&this->m), _mm_setr_ps( inx, iny, inz, inw ) );
};

void Vect::set( const Vect &v ) {
	_mm_store_ps( reinterpret_cast< float* >(&this->m), v.m );
};