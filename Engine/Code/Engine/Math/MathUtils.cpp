#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/PolarCoords.hpp"
#include "Engine/Math/MathUtils.hpp"

#include <stdlib.h>
#include <math.h>


//-----------------------------------------------------------------------------
float Interpolate( float start, float end, float fractionTowardEnd )
{
	return ( ( end - start ) * fractionTowardEnd ) + start;
}


//-----------------------------------------------------------------------------
//Map inValue, from the range inStart to inEnd, to the new range outStart to outEnd.
float RangeMap( float inValue, float inStart, float inEnd, float outStart, float outEnd )
{
	if ( inStart == inEnd ) 
		return 0; //Print warning message when able.

	return ( ( ( inValue - inStart ) / ( inEnd - inStart ) ) * ( outEnd - outStart ) ) + outStart;

}


//-----------------------------------------------------------------------------
float CalcFractionInRange( float inValue, float rangeStart, float rangeEnd )
{
	if ( rangeEnd - rangeStart == 0 ) 
		return -1.f; //PRINT ERROR.

	return (inValue - rangeStart) / (rangeEnd - rangeStart);
}


//-----------------------------------------------------------------------------
float ClampFloat( float inValue, float min, float max )
{
	if ( inValue < min ) return min;
	if ( inValue > max ) return max;
	return inValue;
}


//-----------------------------------------------------------------------------
float ClampFloatZeroToOne( float inValue )
{
	return ClampFloat( inValue, 0.f, 1.f );
}


//-----------------------------------------------------------------------------
float DegreesToRadians( float degrees )
{
	return degrees * ( PI / 180.f );
}


//-----------------------------------------------------------------------------
float RadiansToDegrees( float radians )
{
	return radians * ( 180.f / PI );
}


//-----------------------------------------------------------------------------
float CosDegrees( float degrees )
{
	return cos( DegreesToRadians( degrees ) );
}


//-----------------------------------------------------------------------------
float SinDegrees( float degrees )
{
	return sin( DegreesToRadians( degrees ) );
}


//-----------------------------------------------------------------------------
float Atan2Degrees( const Vector2& direction )
{
	return atan2( direction.y, direction.x );
}


//-----------------------------------------------------------------------------
float CalcDistBetweenPoints( const Vector2& pos1, const Vector2& pos2 )
{
	float xDistance = pos2.x - pos1.x;
	float yDistance = pos2.y - pos1.y;
	return sqrt( ( xDistance * xDistance ) + ( yDistance * yDistance ) );
}


//-----------------------------------------------------------------------------
float CalcDistSquaredBetweenPoints( const Vector2& pos1, const Vector2& pos2 )
{
	float xDistance = pos2.x - pos1.x;
	float yDistance = pos2.y - pos1.y;
	return ( xDistance * xDistance ) + ( yDistance * yDistance );
}


//-----------------------------------------------------------------------------
bool DoDiscsOverlap( const Vector2& center1, float radius1, const Vector2& center2, float radius2 )
{
	float distanceSquared = CalcDistSquaredBetweenPoints( center1, center2 );
	float radiiSum = radius1 + radius2;
	return distanceSquared < ( radiiSum * radiiSum );
}


//-----------------------------------------------------------------------------
bool IsPointInDisc( const Vector2& point, const Vector2& discCenter, float discRadius )
{
	if ( ( point - discCenter ).CalcLength() < discRadius ) return true;
	else return false;
}


//-----------------------------------------------------------------------------
float GetRandomFloatZeroToOne()
{
	return rand() / static_cast<float>( RAND_MAX );
}


//-----------------------------------------------------------------------------
float GetRandomFloatInRange( float min, float max )
{
	return RangeMap( static_cast<float>( rand() ), 0.f, static_cast<float>( RAND_MAX ), min, max );
}


//-----------------------------------------------------------------------------
bool CheckRandomChance( float zeroToOneChanceForSuccess )
{
	return zeroToOneChanceForSuccess >= GetRandomFloatZeroToOne( ) ;
}
