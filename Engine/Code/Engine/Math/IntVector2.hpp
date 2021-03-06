#pragma once

class IntVector2
{
public:
	int x;
	int y;

	IntVector2();
	IntVector2( int initialX, int initialY );
	IntVector2( const IntVector2& vectorToCopy );

	bool operator==( const IntVector2& compareTo ) const;
};