#pragma once

struct Rgba
{
	float red;
	float green;
	float blue;
	float alphaOpacity;
	Rgba() { red = green = blue = alphaOpacity = 1.0f; }
	Rgba( float red, float green, float blue, float alphaOpacity = 1.f )
		: red( red ), blue( blue ), green( green ), alphaOpacity( alphaOpacity ) {}
};