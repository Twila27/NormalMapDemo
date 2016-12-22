#pragma once
#include "Engine/Renderer/Rgba.hpp"


//-----------------------------------------------------------------------------
class TheRenderer;
class Texture;
class Vector2;
class AABB2;


//-----------------------------------------------------------------------------
extern TheRenderer* g_theRenderer;

//-----------------------------------------------------------------------------
class TheRenderer
{
public:
	void ClearScreenToColor( float red, float green, float blue );
	void SetDrawColor( float red, float green, float blue, float opacity ); 
	void SetLineWidth( float newLineWidth );
	void SetBlendFunc( int sourceBlend, int destinationBlend );
	void SetRenderFlag( int flagNameToSet );
	void SetOrtho( const Vector2& bottomLeft, const Vector2& topRight );
	void TranslateView( const Vector2& translation );
	void RotateViewByDegrees( float degrees ); // since in 2D we're always rotating "around the +Z axis"
	void RotateViewByRadians( float radians );
	void ScaleView( float uniformScale );
	void PushView();
	void PopView();
	void DrawLine( const Vector2& startPos, const Vector2& endPos, const Rgba& startColor = Rgba(), const Rgba& endColor = Rgba(), float lineThickness = 1.f );
	void DrawTexturedAABB( const AABB2& bounds, const Texture& texture, const Vector2& texCoordMins, const Vector2& texCoordMaxs, const Rgba& tint = Rgba(), float lineThickness = 1.f );
	void DrawAABB( const AABB2& bounds, const Rgba& color = Rgba(), float lineThickness = 1.f );
	void DrawQuad( const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomRight, const Vector2& bottomLeft, const Rgba& color = Rgba( ), float lineThickness = 1.f );
	void DrawQuadOutline( const Vector2& topLeft, const Vector2& topRight, const Vector2& bottomRight, const Vector2& bottomLeft, const Rgba& color = Rgba(), float lineThickness = 1.f );
	void DrawPolygon( const Vector2& centerPos, float radius, float numSides, float degreesOffset, const Rgba& color = Rgba( ), float lineThickness = 1.f);
};