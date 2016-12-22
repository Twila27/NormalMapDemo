#include "Engine/Math/Vector2.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library

#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/AABB2.hpp"

#include <math.h>


//-----------------------------------------------------------------------------
TheRenderer* g_theRenderer = nullptr;


void TheRenderer::ClearScreenToColor( float red, float green, float blue )
{
	glClearColor( red, green, blue, 1.f ); 
	glClear( GL_COLOR_BUFFER_BIT );
}

void TheRenderer::SetDrawColor( float red, float green, float blue, float opacity )
{
	glColor4f( red, green, blue, opacity );
}

void TheRenderer::SetLineWidth( float newLineWidth )
{
	glLineWidth( newLineWidth );
}

void TheRenderer::SetBlendFunc( int sourceBlend, int destinationBlend )
{
	glBlendFunc( sourceBlend, destinationBlend );
}

void TheRenderer::SetRenderFlag( int flagNameToSet )
{
	glEnable( flagNameToSet );
}

void TheRenderer::SetOrtho( const Vector2& bottomLeft, const Vector2& topRight )
{
	glLoadIdentity( ); 
	glOrtho( bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, 0.f, 1.f );
}

void TheRenderer::TranslateView( const Vector2& translation )
{
	glTranslatef( translation.x, translation.y, 0.f );
}

void TheRenderer::RotateViewByDegrees( float degrees )
{
	glRotatef( degrees, 0.f, 0.f, 1.f );
}

void TheRenderer::RotateViewByRadians( float radians )
{
	glRotatef( RadiansToDegrees(radians) , 0.f, 0.f, 1.f );
}

void TheRenderer::ScaleView( float uniformScale )
{
	glScalef( uniformScale, uniformScale, 1.f );
}

void TheRenderer::PushView()
{
	glPushMatrix( );
}

void TheRenderer::PopView()
{
	glPopMatrix( );
}

//-----------------------------------------------------------------------------
void TheRenderer::DrawLine( const Vector2& startPos, const Vector2& endPos, 
	const Rgba& startColor /*=Rgba()*/, const Rgba& endColor /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	glLineWidth( lineThickness );
	glBegin( GL_LINES );
	{
		glColor4f( startColor.red, startColor.green, startColor.blue, startColor.alphaOpacity );
		glVertex2f( startPos.x, startPos.y );

		glColor4f( endColor.red, endColor.green, endColor.blue, endColor.alphaOpacity );
		glVertex2f( endPos.x, endPos.y );
	}
	glEnd();
}


//-----------------------------------------------------------------------------
//IMPORTANT: vertices iterated CCW from bottom-left, but texels from TOP-left.
void TheRenderer::DrawTexturedAABB( const AABB2& bounds, const Texture& texture,
	const Vector2& texCoordMins, const Vector2& texCoordMaxs, const Rgba& tint  /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, texture.GetTextureID() );
	glColor4f( tint.red, tint.green, tint.blue, tint.alphaOpacity );
	glLineWidth( lineThickness );
	glBegin( GL_QUADS );
	{
		glTexCoord2f( texCoordMins.x, texCoordMaxs.y );
		glVertex2f( bounds.mins.x, bounds.mins.y );

		glTexCoord2f( texCoordMaxs.x, texCoordMaxs.y );
		glVertex2f( bounds.maxs.x, bounds.mins.y );

		glTexCoord2f( texCoordMaxs.x, texCoordMins.y );
		glVertex2f( bounds.maxs.x, bounds.maxs.y );

		glTexCoord2f( texCoordMins.x, texCoordMins.y );
		glVertex2f( bounds.mins.x, bounds.maxs.y );
	}
	glEnd();
	glDisable( GL_TEXTURE_2D );
}


//-----------------------------------------------------------------------------
void TheRenderer::DrawAABB( const AABB2& bounds, const Rgba& color /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	glColor4f( color.red, color.green, color.blue, color.alphaOpacity );
	glLineWidth( lineThickness );
	glBegin( GL_QUADS );
	{
		glVertex2f( bounds.mins.x, bounds.mins.y );;
		glVertex2f( bounds.maxs.x, bounds.mins.y );;
		glVertex2f( bounds.maxs.x, bounds.maxs.y );;
		glVertex2f( bounds.mins.x, bounds.maxs.y );
	}
	glEnd();
}


//-----------------------------------------------------------------------------
void TheRenderer::DrawQuad( const Vector2& topLeft, const Vector2& topRight, 
	const Vector2& bottomRight, const Vector2& bottomLeft, const Rgba& color /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	glColor4f( color.red, color.green, color.blue, color.alphaOpacity );
	glLineWidth( lineThickness );
	glBegin( GL_QUADS );
	{
		glVertex2f( topLeft.x, topLeft.y );
		glVertex2f( topRight.x, topRight.y );
		glVertex2f( bottomRight.x, bottomRight.y );
		glVertex2f( bottomLeft.x, bottomLeft.y );
	}
	glEnd();
}


//-----------------------------------------------------------------------------
void TheRenderer::DrawQuadOutline( const Vector2& topLeft, const Vector2& topRight, 
	const Vector2& bottomRight, const Vector2& bottomLeft, const Rgba& color /*=Rgba()*/, float lineThickness /*= 1.f */ )
{
	glColor4f( color.red, color.green, color.blue, color.alphaOpacity );
	glLineWidth( lineThickness );
	glBegin( GL_LINE_LOOP );
	{
		glVertex2f( topLeft.x, topLeft.y );
		glVertex2f( topRight.x, topRight.y );
		glVertex2f( bottomRight.x, bottomRight.y );
		glVertex2f( bottomLeft.x, bottomLeft.y );
	}
	glEnd();
}


//-----------------------------------------------------------------------------
void TheRenderer::DrawPolygon( const Vector2& centerPos, float radius, float numSides, float degreesOffset, const Rgba& color /*=Rgba()*/, float lineThickness /*=1.0f*/)
{
	const float radiansTotal = 2.f * PI;
	const float radiansPerSide = radiansTotal / numSides;
	glColor4f( color.red, color.green, color.blue, color.alphaOpacity );
	glLineWidth( lineThickness );
	glBegin( GL_LINE_LOOP );
	{		
		for ( float radians = 0.f; radians < radiansTotal; radians += radiansPerSide ) {
			float rotatedRadians = radians + DegreesToRadians( degreesOffset );
			float x = centerPos.x + ( radius * cos( rotatedRadians ) );
			float y = centerPos.y + ( radius * sin( rotatedRadians ) );
			glVertex2f( x, y );
		}
	}
	glEnd();
}