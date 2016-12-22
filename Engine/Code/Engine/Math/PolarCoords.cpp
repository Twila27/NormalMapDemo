#include "Engine/Math/PolarCoords.hpp"


//--------------------------------------------------------------------------------------------------------------
// The spec asks for a "do-nothing" default ctor that does NOT initialize x and y.
PolarCoords::PolarCoords()
: radius()
, thetaRadians()
{
}


//--------------------------------------------------------------------------------------------------------------
PolarCoords::PolarCoords( float initialRadius, float initialThetaRadians )
: radius( initialRadius )
, thetaRadians( initialThetaRadians )
{
}


//--------------------------------------------------------------------------------------------------------------
PolarCoords::PolarCoords( const PolarCoords& coordsToCopy )
: radius( coordsToCopy.radius )
, thetaRadians( coordsToCopy.thetaRadians )
{
}
