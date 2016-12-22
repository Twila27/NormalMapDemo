#pragma once

//--------------------------------------------------
class Vector2
{
public:
	Vector2();
	Vector2( float initialX, float initialY );
	Vector2( const Vector2& vectorToCopy );

	void GetXY( float& out_x, float& out_y ) const;
	float CalcLength() const;
	Vector2 Normalize();

	void SetXY( float newX, float newY );

	Vector2 operator+( const Vector2& vectorToAdd ) const;
	Vector2 operator-( const Vector2& vectorToAdd ) const;
	Vector2 operator-( );
	Vector2 operator*( float scalarToScaleBy ) const;
	Vector2 operator/( float scalarToScaleBy ) const;
	void operator+=( const Vector2& vectorToAdd );
	void operator-=( const Vector2& vectorToSubtract );
	void operator*=( const float uniformScale );
	void operator=( const Vector2& copyFrom );
	bool operator==( const Vector2& compareTo ) const;

public:
	float x;
	float y;
};