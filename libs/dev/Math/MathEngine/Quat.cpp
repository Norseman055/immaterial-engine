/*****************************************************************************/
/*                                                                           */
/* File: Quat.cpp                                                            */
/*                                                                           */
/* Quaternion Class                                                           */
/*                                                                           */
/*****************************************************************************/

#include "Quat.h"
#include <math.h>

Quat::Quat() {
	this->m = _mm_setr_ps( 0, 0, 0, 1 );
}

Quat::Quat( const Quat &q0 ) {
	this->m = q0.m;
}

Quat::Quat( const __m128& m0 ) {
	this->m = m0;
}

Quat::Quat( const Matrix &M ) {
	auto tr = M._m0 + M._m5 + M._m10;
	float S;

	if ( tr > 0.0f ) {
		S = 2 * sqrtf( tr + 1.0f );
		this->m = _mm_setr_ps(
			-(M._m9 - M._m6) / S,
			-(M._m2 - M._m8) / S,
			-(M._m4 - M._m1) / S,
			0.25f * S );
	} else if ( (M._m0 > M._m5) && (M._m0 > M._m10) ) {
		S = 2 * sqrtf( 1.0f + M._m0 - M._m5 - M._m10 );
		this->m = _mm_setr_ps(
			0.25f * S,
			(M._m1 + M._m4) / S,
			(M._m2 + M._m8) / S,
			-(M._m9 - M._m6) / S );
	} else if ( M._m5 > M._m10 ) {
		S = 2 * sqrtf( 1.0f + M._m5 - M._m0 - M._m10 );
		this->m = _mm_setr_ps(
			(M._m1 + M._m4) / S,
			0.25f * S,
			(M._m6 + M._m9) / S,
			-(M._m2 - M._m8) / S );
	} else {
		S = 2 * sqrtf( 1.0f + M._m10 - M._m0 - M._m5 );
		this->m = _mm_setr_ps(
			(M._m2 + M._m8) / S,
			(M._m6 + M._m9) / S,
			0.25f * S,
			-(M._m4 - M._m1) / S );
	}
};

Quat::Quat( const float f0, const float f1, const float f2, const float f3 ) {
	this->m = _mm_setr_ps( f0, f1, f2, f3 );
}

Quat::Quat( const Vect &v, const float f0 ) {
	this->m = _mm_setr_ps( v.vx, v.vy, v.vz, f0 );
}

Quat::Quat( const Rot3AxisType, const float f0, const float f1, const float f2 ) {
	*this = Quat( Matrix( ROT_XYZ, f0, f1, f2 ) );
};

Quat::Quat( const RotType in, const float f0 ) {
	*this = Quat( Matrix( in, f0 ) );
};

Quat::Quat( const RotAxisAngleType, const Vect &v, const float f0 ) {
	*this = Quat( Matrix( ROT_AXIS_ANGLE, v, f0 ) );
};

Quat::Quat( const RotOrientType in, const Vect &v0, const Vect &v1 ) {
	*this = Quat( Matrix( in, v0, v1 ) );
}

Quat::Quat( MatrixSpecialType in ) {
	switch ( in ) {
		case ZERO:
			// All zeros
			this->m = _mm_setzero_ps();
			break;
		default:
			// Identity, all zero except 1 at end
			this->m = _mm_setr_ps( 0, 0, 0, 1 );
			break;
	}
}

Quat::~Quat() { }

float Quat::operator[]( const x_enum )const {
	return this->qx;
};

float Quat::operator[]( const y_enum )const {
	return this->qy;
};

float Quat::operator[]( const z_enum )const {
	return this->qz;
};

float Quat::operator[]( const w_enum )const {
	return this->qw;
};

float& Quat::operator[]( const x_enum ) {
	return this->qx;
};

float& Quat::operator[]( const y_enum ) {
	return this->qy;
};

float& Quat::operator[]( const z_enum ) {
	return this->qz;
};

float& Quat::operator[]( const w_enum ) {
	return this->qw;
};

void Quat::set( const float f0, const float f1, const float f2, const float f3 ) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_setr_ps( f0, f1, f2, f3 ) );
};

void Quat::set( const Matrix &_m ) {
	*this = Quat( _m );
};

void Quat::set( const RotType in, const float f ) {
	switch ( in ) {
		case ROT_X:
			*this = Quat( Matrix( ROT_X, f ) );
			break;
		case ROT_Y:
			*this = Quat( Matrix( ROT_Y, f ) );
			break;
		case ROT_Z:
			*this = Quat( Matrix( ROT_Z, f ) );
			break;
	}
};

void Quat::set( const Vect &v, const float f ) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_setr_ps( v.vx, v.vy, v.vz, f ) );
};

void Quat::set( const __m128& xm ) {
	_mm_store_ps( reinterpret_cast< float* >(&m), xm );
}

void Quat::set( const RotAxisAngleType, const Vect &v, const float f0 ) {
	*this = Quat( Matrix( ROT_AXIS_ANGLE, v, f0 ) );
};

void Quat::set( const RotOrientType in, const Vect &v0, const Vect &v1 ) {
	*this = Quat( Matrix( in, v0, v1 ) );
};

void Quat::set( const MatrixSpecialType in ) {
	switch ( in ) {
		case ZERO:
			_mm_store_ps( reinterpret_cast< float* >(&m), _mm_setzero_ps() );
			break;
		case IDENTITY:
			_mm_store_ps( reinterpret_cast< float* >(&m), _mm_setr_ps( 0, 0, 0, 1 ) );
			break;
	}
};

void Quat::set( const Rot3AxisType, const float f0, const float f1, const float f2 ) {
	*this = Quat( Matrix( ROT_XYZ, f0, f1, f2 ) );
};

void Quat::setVect( const Vect &v0 ) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_setr_ps( v0.vx, v0.vy, v0.vz, this->qw ) );
};

void Quat::getVect( Vect &v0 ) const {
	_mm_store_ps( reinterpret_cast< float* >(&v0.m), _mm_setr_ps( this->qx, this->qy, this->qz, v0.vw ) );
};

void Quat::operator=(const Quat &q) {
	_mm_store_ps( reinterpret_cast< float* >(&m), q.m );
};

Quat Quat::operator+(const Quat &q)const {
	return Quat( _mm_add_ps( this->m, q.m ) );
};

Quat Quat::operator+(const float f0)const {
	return Quat( _mm_add_ps( this->m, _mm_set_ps1( f0 ) ) );
};

void Quat::operator+=(const Quat &q) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_add_ps( this->m, q.m ) );;
};

void Quat::operator+=(const float f0) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_add_ps( this->m, _mm_set_ps1( f0 ) ) );
};

Quat operator+(const float f0, const Quat &q) {
	return Quat( _mm_add_ps( q.m, _mm_set_ps1( f0 ) ) );
};

Quat Quat::operator-(const Quat &q)const {
	return Quat( _mm_sub_ps( this->m, q.m ) );
};

Quat Quat::operator-(const float f0)const {
	return Quat( _mm_sub_ps( this->m, _mm_set_ps1( f0 ) ) );
};

void Quat::operator-=(const Quat &q) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_sub_ps( this->m, q.m ) );
};

void Quat::operator-=(const float f0) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_sub_ps( this->m, _mm_set_ps1( f0 ) ) );
};

Quat operator-(const float f0, const Quat &q) {
	return Quat( _mm_sub_ps( _mm_set_ps1( f0 ), q.m ) );
};

Quat Quat::operator*(const Quat &q)const {
	Vect vLeft( this->qx, this->qy, this->qz );
	Vect vRight( q.qx, q.qy, q.qz );
	return Quat( Vect( vRight.cross( vLeft ) + (q.qw * vLeft) + (this->qw * vRight) ), (this->qw * q.qw) - vLeft.dot( vRight ) );
};

Quat Quat::operator*(const float f)const {
	return Quat( _mm_mul_ps( this->m, _mm_set_ps1( f ) ) );
};

void Quat::operator*=(const Quat &q) {
	Vect vLeft( this->qx, this->qy, this->qz );
	Vect vRight( q.qx, q.qy, q.qz );
	*this = Quat( Vect( vRight.cross( vLeft ) + (q.qw * vLeft) + (this->qw * vRight) ), (this->qw * q.qw) - vLeft.dot( vRight ) );
};

void Quat::operator*=(const float f) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_mul_ps( this->m, _mm_set_ps1( f ) ) );
};

void Quat::operator*=(const Matrix &_m) {
	this->set( Matrix( *this ) * _m );
};

Quat operator*(const float f, const Quat &q) {
	return Quat( _mm_mul_ps( q.m, _mm_set_ps1( f ) ) );
};

Quat Quat::operator/(const Quat &q)const {
	return Quat( _mm_div_ps( this->m, q.m ) );
};

Quat Quat::operator/(const float f0)const {
	return Quat( _mm_div_ps( this->m, _mm_set_ps1( f0 ) ) );
};

void Quat::operator/=(const Quat &q) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_div_ps( this->m, q.m ) );
};

void Quat::operator/=(const float f0) {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_div_ps( this->m, _mm_set_ps1( f0 ) ) );
};

Quat operator/(const float f0, const Quat &q) {
	return Quat( _mm_div_ps( _mm_set_ps1( f0 ), q.m ) );
};

Quat Quat::operator-() const {
	return Quat( *this * -1 );
};

Quat Quat::operator+() const {
	return *this;
};

Quat Quat::multByElement( const Quat &q ) const {
	return Quat( _mm_mul_ps( this->m, q.m ) );
};

Quat Quat::getConj()const {
	return Quat( _mm_mul_ps( this->m, _mm_setr_ps( -1, -1, -1, 1 ) ) );
};

Quat Quat::getT() const {
	return Quat( Matrix( *this ).getT() );
};

Quat Quat::norm() {
	auto mag = sqrtf( this->qx * this->qx + this->qy * this->qy + this->qz * this->qz + this->qw * this->qw );
	this->set( _mm_div_ps( this->m, _mm_set_ps1( mag ) ) );
	return Quat( this->m );
};

Quat Quat::getNorm()const {
	auto mag = sqrtf( this->qx * this->qx + this->qy * this->qy + this->qz * this->qz + this->qw * this->qw );
	return Quat( _mm_div_ps( this->m, _mm_set_ps1( mag ) ) );
};

Quat Quat::getInv()const {
	return Quat( *this / this->magSquared() ).getConj();
};

void Quat::conj() {
	_mm_store_ps( reinterpret_cast< float* >(&m), _mm_mul_ps( this->m, _mm_setr_ps( -1, -1, -1, 1 ) ) );
};

void Quat::T() {
	*this = Quat( Matrix( *this ).getT() );
};

void Quat::inv() {
	*this = Quat( _mm_div_ps( this->m, _mm_set_ps1( this->magSquared() ) ) ).getConj();
};

void Quat::Lqcvq( const Vect &vIn, Vect &vOut ) const {
	Vect tmp( this->qx, this->qy, this->qz );
	vOut = 2 * this->qw * tmp.cross( vIn ) + (this->qw * this->qw - tmp.dot( tmp )) * vIn + 2 * tmp.dot( vIn ) * tmp;
};

void Quat::Lqvqc( const Vect &vIn, Vect &vOut ) const {
	Vect tmp( this->qx, this->qy, this->qz );
	vOut = 2 * this->qw * vIn.cross( tmp ) + (this->qw * this->qw - tmp.dot( tmp )) * vIn + 2 * tmp.dot( vIn ) * tmp;
};

void Quat::getAxis( Vect &v ) const {
	_mm_store_ps( reinterpret_cast< float* >(&v.m), _mm_setr_ps( qx, qy, qz, v.vw ) );
};

float Quat::mag()const {
	return sqrtf( this->magSquared() );
};

float Quat::magSquared()const {
	return this->qx * this->qx + this->qy * this->qy + this->qz * this->qz + this->qw * this->qw;
};

float Quat::invMag()const {
	return 1 / this->mag();
};

float Quat::dot( const Quat &q )const {
	return this->qx * q.qx + this->qy * q.qy + this->qz * q.qz + this->qw * q.qw;
};

float Quat::getAngle()const {
	return 2 * (acosf( this->qw ));
};

bool Quat::isEqual( const Quat &q, const float f0 ) const {
	auto answer = false;
	if ( fabs( this->qx - q.qx ) <= f0 && fabs( this->qy - q.qy ) <= f0 && fabs( this->qz - q.qz ) <= f0 && fabs( this->qw - q.qw ) <= f0 ) {
		answer = true;
	}
	return answer;
};

bool Quat::isNegEqual( const Quat &q, const float f0 ) const {
	auto answer = false;
	if ( this->qx + q.qx <= f0 && this->qy + q.qy <= f0 && this->qz + q.qz <= f0 && this->qw + q.qw <= f0 ) {
		answer = true;
	}
	return answer;
};

bool Quat::isEquivalent( const Quat &q, const float precision ) const {
	auto answer = this->isEqual( q, precision );
	if ( !answer ) {
		answer = this->isEqual( -q, precision );
	}
	return answer;
};

bool Quat::isConjugateEqual( const Quat &q, const float precision ) const {
	auto answer = false;
	if ( this->qx + q.qx <= precision && this->qy + q.qy <= precision && this->qz + q.qz <= precision && this->qw - q.qw <= precision ) {
		answer = true;
	}
	return answer;
};

bool Quat::isIdentity( const float precision ) const {
	auto answer = false;
	if ( fabs( this->qx ) - precision <= 0 && fabs( this->qy ) - precision <= 0 && fabs( this->qz ) - precision <= 0 && fabs( this->qw ) - 1 - precision <= 0 ) {
		answer = true;
	}
	return answer;
};

bool Quat::isNormalized( const float precision ) const {
	auto answer = false;
	Vect tmp( this->qx, this->qy, this->qz );

	if ( fabs( this->qw * this->qw + tmp.dot( tmp ) ) - 1 <= precision ) {
		answer = true;
	}
	return answer;
};

bool Quat::isZero( const float precision ) const {
	auto answer = false;
	if ( fabs( this->qx ) <= precision && fabs( this->qy ) <= precision && fabs( this->qz ) <= precision && fabs( this->qw ) <= precision ) {
		answer = true;
	}
	return answer;
};