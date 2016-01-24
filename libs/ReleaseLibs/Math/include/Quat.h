/** @file */
/*****************************************************************************/
/*                                                                           */
/* file: Quat.h                                                              */
/*                                                                           */
/*****************************************************************************/

#ifndef ENGINE_MATH_QUAT_H
#define ENGINE_MATH_QUAT_H

/*****************************************************************************/
/* Includes:                                                                 */
/*****************************************************************************/
#include "Matrix.h"
#include "Enum.h"
#include "Vect.h"

/*************************************************************************
*
* This class contains functions and data structure for a 4-dimensional
* Quaternions.  The quaternions are internally stored interms of the
* imaginary components (qx,qy,qz) and the real components (qw).
*
* In the Math library, quaternions can be thought of as "rotational"
* matrices, everything that you can represent with rotational matrices
* you can use with quaternions.  Such as quaternion concatenation,
* inverses, transpose, normalize, multiplying with matrices, and with
* vectors.
*
*
**************************************************************************/

class Quat final : public Align16 {
public:

	Quat();
	Quat( const Quat &q0 );
	Quat( const Matrix &m0 );
	Quat( const float f0, const float f1, const float f2, const float f3 );
	Quat( const Vect &v, const float f0 );
	Quat( const Rot3AxisType, const float f0, const float f1, const float f2 );
	Quat( const RotType in, const float f0 );
	Quat( const RotAxisAngleType, const Vect &v, const float f0 );
	Quat( const RotOrientType in, const Vect &v0, const Vect &v1 );

	const float operator[]( const x_enum )const;
	const float operator[]( const y_enum )const;
	const float operator[]( const z_enum )const;
	const float operator[]( const w_enum )const;
	float& operator[]( const x_enum );
	float& operator[]( const y_enum );
	float& operator[]( const z_enum );
	float& operator[]( const w_enum );

	const void set( const float f0, const float f1, const float f2, const float f3 );
	const void set( const Matrix &m );
	const void set( const RotType in, const float f );
	const void set( const Vect &v, const float f );
	const void set( const RotAxisAngleType, const Vect &v, const float f0 );
	const void set( const RotOrientType in, const Vect &v0, const Vect &v1 );
	const void set( const MatrixSpecialType in );
	const void set( const Rot3AxisType, const float f0, const float f1, const float f2 );

	const void setVect( const Vect &v0 );
	const void getVect( Vect &v0 );

	// binary
	const void operator = (const Quat &q);
	const Quat operator + (const Quat &q)const;
	const Quat operator + (const float f0)const;
	const void operator += (const Quat &q);
	const void operator += (const float f0);
	friend const Quat operator + (const float f0, const Quat &t);
	const Quat operator - (const Quat &q)const;
	const Quat operator - (const float f0)const;
	const void operator -= (const Quat &q);
	const void operator -= (const float f0);
	friend const Quat operator - (const float f0, const Quat &q);
	const Quat operator * (const Quat &q)const;
	const Quat operator * (const float f)const;
	const void operator *= (const Quat &q);
	const void operator *= (const float f0);
	const void operator *= (const Matrix &m);
	friend const Quat operator * (const float f0, const Quat &q);
	const Quat operator / (const Quat &q)const;
	const Quat operator / (const float f0)const;
	const void operator /= (const Quat &q);
	const void operator /= (const float f0);
	friend const Quat operator / (const float f0, const Quat &q);

	// unary
	friend const Quat operator - (const Quat &q);
	friend const Quat operator + (const Quat &q);

	const Quat multByElement( const Quat &q );
	const Quat getConj()const;
	const Quat getT();
	const Quat norm();
	const Quat getNorm()const;
	const Quat getInv()const;

	const void conj();
	const void T();
	const void inv();
	const void Lqvqc( const Vect &vIn, Vect &vOut );
	const void Lqcvq( const Vect &vIn, Vect &vOut );
	const void getAxis( Vect &v );

	const float mag()const;
	const float magSquared()const;
	const float invMag()const;
	const float dot( const Quat &q )const;
	const float getAngle()const;

	const bool isEqual( const Quat &q, const float precision );
	const bool isNegEqual( const Quat &q, const float precision );
	const bool isEquivalent( const Quat &q, const float precision );
	const bool isConjugateEqual( const Quat &q, const float precision );
	const bool isIdentity( const float precision );
	const bool isNormalized( const float precision );
	const bool isZero( const float precision );
	// do magic

private:
	friend Matrix;
	// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)

	// anonymous union
	union {
		__m128	m;

		// anonymous struct
		struct {
			float qx;
			float qy;
			float qz;
			float qw;
		};
	};
};

#endif // #ifndef QUAT_H

/**** END of Quat.h ********************************************************/
