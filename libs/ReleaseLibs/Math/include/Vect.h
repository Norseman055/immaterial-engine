#ifndef ENGINE_MATH_VECT_H
#define ENGINE_MATH_VECT_H

#include <smmintrin.h>
#include "Constants.h"
#include "Enum.h"
#include "Align16.h"
// forward declare
class Matrix;
class Quat;

class Vect final : public Align16 {
public:

	Vect();
	Vect( const float x, const float y, const float z, const float w = 1.0f );
	Vect( const __m128 inM );
	~Vect();

	const void operator = (const Vect &t);
	const Vect operator + (const Vect &t)const;
	const void operator += (const Vect &t);
	const Vect operator - (const Vect &t)const;
	const void operator -= (const Vect &t);
	const Vect operator * (const Matrix &t)const;
	const void operator *= (const Matrix &t);
	const void operator *= (const float f);
	const Vect operator * (const float f)const;	// vect * float
	friend const Vect operator * (const float f, const Vect &v);	// float * vect
	friend const Vect operator + (const Vect &t);
	friend const Vect operator - (const Vect &t);

	const float operator[]( const x_enum )const;
	float& operator[]( const x_enum );
	const float operator[]( const y_enum )const;
	float& operator[]( const y_enum );
	const float operator[]( const z_enum )const;
	float& operator[]( const z_enum );
	const float operator[]( const w_enum )const;
	float& operator[]( const w_enum );

	const bool isEqual( const Vect &t, const float &precision = MATH_TOLERANCE )const;
	const bool isZero( const float &precision = 0.01f )const;

	const float dot( const Vect &t )const;
	const Vect cross( const Vect &t )const;
	const void norm();
	const Vect getNorm()const;
	const float mag()const;
	const float magSqr()const;
	const float getAngle( const Vect &t )const;

	const void set( const float x, const float y, const float z, const float w = 1.0f );
	const void set( const Vect &v );

private:
	friend Matrix;
	friend Quat;
	// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)
	// anonymous union
	union {
		__m128	m;

		// anonymous struct
		struct {
			float vx;
			float vy;
			float vz;
			float vw;
		};
	};
};

#endif