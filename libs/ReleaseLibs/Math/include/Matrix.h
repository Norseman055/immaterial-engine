#ifndef ENGINE_MATH_MATRIX_H
#define ENGINE_MATH_MATRIX_H

#include "Enum.h"
#include "Vect.h"
#include "Quat.h"

class Matrix final : public Align16 {
public:
	Matrix();
	Matrix( const Vect &v0, const Vect &v1, const Vect &v2, const Vect &v3 );
	Matrix( const float m0, const float m1, const float m2, const float m3,
			const float m4, const float m5, const float m6, const float m7,
			const float m8, const float m9, const float m10, const float m11,
			const float m12, const float m13, const float m14, const float m15 );
	Matrix( const MatrixSpecialType in );
	Matrix( const MatrixTransType, const float f0, const float f1, const float f2 );
	Matrix( const MatrixTransType, const Vect &v0 );
	Matrix( const MatrixScaleType, const float f0, const float f1, const float f2 );
	Matrix( const MatrixScaleType, const Vect &v0 );
	Matrix( const RotType in, const float f );
	Matrix( const RotAxisAngleType, const Vect &v0, const float f0 );
	Matrix( const Rot3AxisType, const float f0, const float f1, const float f2 );
	Matrix( const RotOrientType in, const Vect &v0, const Vect &v1 );
	Matrix( const Matrix &m0 );
	Matrix( const Quat &q0 );

	~Matrix();

	const void set( const MatrixSpecialType in );
	const void set( const MatrixTransType, const float f0, const float f1, const float f2 );
	const void set( const MatrixTransType, const Vect &v0 );
	const void set( const MatrixScaleType, const float f0, const float f1, const float f2 );
	const void set( const MatrixScaleType, const Vect &v0 );
	const void set( const RotType in, const float f );
	const void set( const Rot3AxisType, const float f0, const float f1, const float f2 );
	const void set( const RotAxisAngleType, const Vect &v0, const float f0 );
	const void set( const RotOrientType in, const Vect &v0, const Vect &v1 );
	const void set( const MatrixRowType in, const Vect &v );
	const void set( const Vect &v0, const Vect &v1, const Vect &v2, const Vect &v3 );
	const void set( const Quat &q0 );

	const Vect get( const MatrixRowType in )const;

	const void operator = (const Matrix &t);
	const Matrix operator + (const Matrix &t)const;
	const void operator += (const Matrix &t);
	const Matrix operator - (const Matrix &t)const;
	const void operator -= (const Matrix &t);
	const Matrix operator * (const Matrix &t)const;
	const void operator *= (const Matrix &t);
	const void operator *= (const float f);
	friend const Matrix operator * (const float f, const Matrix &t);
	friend const Matrix operator * (const Matrix &t, const float f);
	friend const Matrix operator + (const Matrix &t);
	friend const Matrix operator - (const Matrix &t);

	const Vect operator[]( const MatrixRowType )const;
	const float operator[]( const m0_enum )const;
	const float operator[]( const m1_enum )const;
	const float operator[]( const m2_enum )const;
	const float operator[]( const m3_enum )const;
	const float operator[]( const m4_enum )const;
	const float operator[]( const m5_enum )const;
	const float operator[]( const m6_enum )const;
	const float operator[]( const m7_enum )const;
	const float operator[]( const m8_enum )const;
	const float operator[]( const m9_enum )const;
	const float operator[]( const m10_enum )const;
	const float operator[]( const m11_enum )const;
	const float operator[]( const m12_enum )const;
	const float operator[]( const m13_enum )const;
	const float operator[]( const m14_enum )const;
	const float operator[]( const m15_enum )const;
	Vect& operator[]( const MatrixRowType );
	float& operator[]( const m0_enum );
	float& operator[]( const m1_enum );
	float& operator[]( const m2_enum );
	float& operator[]( const m3_enum );
	float& operator[]( const m4_enum );
	float& operator[]( const m5_enum );
	float& operator[]( const m6_enum );
	float& operator[]( const m7_enum );
	float& operator[]( const m8_enum );
	float& operator[]( const m9_enum );
	float& operator[]( const m10_enum );
	float& operator[]( const m11_enum );
	float& operator[]( const m12_enum );
	float& operator[]( const m13_enum );
	float& operator[]( const m14_enum );
	float& operator[]( const m15_enum );

	const bool isEqual( const Matrix &t )const;
	const bool isIdentity( const float f = MATH_TOLERANCE )const;

	const float det()const;
	const void T();
	const Matrix getT()const;
	const void inv();
	const Matrix getInv()const;

private:
	friend Vect;
	friend Quat;
	// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)

	union {
		struct {
			Vect v0;
			Vect v1;
			Vect v2;
			Vect v3;
		};

		struct {
			float _m0;
			float _m1;
			float _m2;
			float _m3;
			float _m4;
			float _m5;
			float _m6;
			float _m7;
			float _m8;
			float _m9;
			float _m10;
			float _m11;
			float _m12;
			float _m13;
			float _m14;
			float _m15;
		};
	};
};

#endif