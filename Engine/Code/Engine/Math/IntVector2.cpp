#include "Engine/Math/IntVector2.hpp"

IntVector2::IntVector2()
	: x()
	, y()
{
}


//-----------------------------------------------------------------------------
IntVector2::IntVector2( int initialX, int initialY )
	: x( initialX )
	, y( initialY )
{
}


//-----------------------------------------------------------------------------
IntVector2::IntVector2( const IntVector2& vectorToCopy )
	: x( vectorToCopy.x )
	, y( vectorToCopy.y )
{
}

//-----------------------------------------------------------------------------
bool IntVector2::operator==( const IntVector2& compareTo ) const
{
	return ( x == compareTo.x ) & ( y == compareTo.y );
}
