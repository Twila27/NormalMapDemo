#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include <stdlib.h>
#include <time.h>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library

#include "Engine/Error/ErrorWarningAssert.hpp"
#include "Engine/Renderer/TheRenderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Time/Time.hpp"
#include "Game/TheApp.hpp"
#include "Game/TheGame.hpp"

//Just for this project.
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"

//For image loading and writing.
#define STBI_HEADER_FILE_ONLY
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image.h"
#include "ThirdParty/stb/stb_image_write.h"

//-----------------------------------------------------------------------------------------------
#define UNUSED(x) (void)(x);


//-----------------------------------------------------------------------------------------------
//Noting that this sets (0,0) is in the bottom-left until SetOrtho is called later on.
const int OFFSET_FROM_WINDOWS_DESKTOP = 50;
const int WINDOW_PHYSICAL_WIDTH = 1600;
const int WINDOW_PHYSICAL_HEIGHT = 900;
const float VIEW_LEFT = 0.0;
const float VIEW_RIGHT = 1600.0;
const float VIEW_BOTTOM = 0.0;
const float VIEW_TOP = VIEW_RIGHT * static_cast<float>( WINDOW_PHYSICAL_HEIGHT ) / static_cast<float>( WINDOW_PHYSICAL_WIDTH );

float g_spinCounter;
float g_bKeyStopwatch;
bool g_isQuitting = false;
HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "Assault (Milestone 01) by Benjamin Gibson";


//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure( HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	unsigned char asKey = (unsigned char)wParam;
	switch ( wmMessageCode )
	{
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		g_isQuitting = true;
		return 0;

	case WM_KEYDOWN:
		g_theApp->SetKeyDownStatus( asKey, true );
		if ( asKey == VK_ESCAPE )
		{
			g_isQuitting = true;
			return 0;
		}
		//		g_keyDownArray[ asKey ] = true;
		break;

	case WM_KEYUP:
		g_theApp->SetKeyDownStatus( asKey, false );
		//		g_keyDownArray[ asKey ] = false;
		break;
	}

	return DefWindowProc( windowHandle, wmMessageCode, wParam, lParam );
}


//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow( HINSTANCE applicationInstanceHandle )
{
	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset( &windowClassDescription, 0, sizeof( windowClassDescription ) );
	windowClassDescription.cbSize = sizeof( windowClassDescription );
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>( WindowsMessageHandlingProcedure ); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle( NULL );
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT( "Simple Window Class" );
	RegisterClassEx( &windowClassDescription );

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow( );
	GetClientRect( desktopWindowHandle, &desktopRect );

	RECT windowRect = { OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_WIDTH, OFFSET_FROM_WINDOWS_DESKTOP + WINDOW_PHYSICAL_HEIGHT };
	AdjustWindowRectEx( &windowRect, windowStyleFlags, FALSE, windowStyleExFlags );

	WCHAR windowTitle[ 1024 ];
	MultiByteToWideChar( GetACP( ), 0, APP_NAME, -1, windowTitle, sizeof( windowTitle ) / sizeof( windowTitle[ 0 ] ) );
	g_hWnd = CreateWindowEx(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL );

	ShowWindow( g_hWnd, SW_SHOW );
	SetForegroundWindow( g_hWnd );
	SetFocus( g_hWnd );

	g_displayDeviceContext = GetDC( g_hWnd );

	HCURSOR cursor = LoadCursor( NULL, IDC_ARROW );
	SetCursor( cursor );

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset( &pixelFormatDescriptor, 0, sizeof( pixelFormatDescriptor ) );
	pixelFormatDescriptor.nSize = sizeof( pixelFormatDescriptor );
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 24;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cAccumBits = 0;
	pixelFormatDescriptor.cStencilBits = 8;

	int pixelFormatCode = ChoosePixelFormat( g_displayDeviceContext, &pixelFormatDescriptor );
	SetPixelFormat( g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor );
	g_openGLRenderingContext = wglCreateContext( g_displayDeviceContext );
	wglMakeCurrent( g_displayDeviceContext, g_openGLRenderingContext );

	g_theRenderer->SetOrtho( Vector2( VIEW_LEFT, VIEW_BOTTOM ), Vector2( VIEW_RIGHT, VIEW_TOP ) );
	g_theRenderer->SetRenderFlag( GL_BLEND );
	g_theRenderer->SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	g_theRenderer->SetLineWidth( 2.f );
	g_theRenderer->SetRenderFlag( GL_LINE_SMOOTH );
}


//-----------------------------------------------------------------------------------------------
void RunMessagePump( )
{
	MSG queuedMessage;
	for ( ;; )
	{
		const BOOL wasMessagePresent = PeekMessage( &queuedMessage, NULL, 0, 0, PM_REMOVE );
		if ( !wasMessagePresent )
		{
			break;
		}

		TranslateMessage( &queuedMessage );
		DispatchMessage( &queuedMessage );
	}
}


//-----------------------------------------------------------------------------------------------
unsigned int g_outputTextureID = 0;
void CalculateDOT3(float lightDirX, float lightDirY, float lightDirZ)
{
	//Thanks in large part to Squirrel Eiserloh's texture image loading code.
	//Input: a diffuse texture and a bump map for it (can be cool when they don't match up 1:1).
	//Input: a vector2 direction of light vector.
	//Output: a texture (image file) where each texel fragment_diffuse_color := diffuse_map_color * max( 0, dot( normal_map_value, light_direction_vector ) ).

	int numComponents( 0 ); //Filled by image load call.
	int numComponentsRequested( 3 ); //Says RGB or RGBA will be supported.
	IntVector2 diffuseTextureSize;
	IntVector2 bumpTextureSize;
	IntVector2 outputTextureSize;
	unsigned char* diffuseTextureData = stbi_load( static_cast<const char*>( "Data/Images/diffuseTexture.png" ), &diffuseTextureSize.x, &diffuseTextureSize.y, &numComponents, numComponentsRequested );
	unsigned char* bumpTextureData = stbi_load( static_cast<const char*>( "Data/Images/bumpTexture.png" ), &bumpTextureSize.x, &bumpTextureSize.y, &numComponents, numComponentsRequested );
	ASSERT_OR_DIE( diffuseTextureSize == bumpTextureSize, "ERROR: Please supply a diffuse texture and bump map of equal width and height." );
	outputTextureSize = diffuseTextureSize;

	//Note pixel color components are unsigned bytes, i.e. 1 byte (unsigned char in C++) per channel.
	int numTexels = outputTextureSize.x * outputTextureSize.y;
	int numBytes = numTexels * numComponents;
	unsigned char* outputImageData = new unsigned char[ numBytes ];
	for ( int i = 0; i < numBytes; i += numComponents ) //Loop over all bytes, not just # texels.
	{
		//Handle dot product side, being sure to make both 3D vectors unit length.
		float normalDir[ ] = { 
				RangeMap( bumpTextureData[ i ], 0.f, 255.f, -1.f, 1.f ), 
				RangeMap( bumpTextureData[ i+1 ], 0.f, 255.f, -1.f, 1.f ), 
				RangeMap( bumpTextureData[ i+2 ], 0.f, 255.f, -1.f, 1.f ), 
		};
		float normalDirLen = sqrt( normalDir[ 0 ] * normalDir[ 0 ] ) + ( normalDir[ 1 ] * normalDir[ 1 ] ) + ( normalDir[ 2 ] * normalDir[ 2 ] );
		{
			normalDir[ 0 ] /= normalDirLen;
			normalDir[ 1 ] /= normalDirLen;
			normalDir[ 2 ] /= normalDirLen;
		}
		float lightDir[] = { lightDirX / 255.f, lightDirY / 255.f, lightDirZ / 255.f }; //NOTE: lightDir args clamped [0,255] in Update() below.
		float lightDirLen = sqrt( normalDir[ 0 ] * normalDir[ 0 ] ) + ( normalDir[ 1 ] * normalDir[ 1 ] ) + ( normalDir[ 2 ] * normalDir[ 2 ] );
		{
			lightDir[ 0 ] /= lightDirLen;
			lightDir[ 1 ] /= lightDirLen;
			lightDir[ 2 ] /= lightDirLen;
		}
		float NdotL = ( normalDir[ 0 ] * lightDir[ 0 ] ) + ( normalDir[ 1 ] * lightDir[ 1 ] ) + ( normalDir[ 2 ] * lightDir[ 2 ] );
		float clampedNdotL = ClampFloatZeroToOne( NdotL );

		//Compute DOT3 for this RGB grouping via vector multiplication by a scalar.
		outputImageData[ i ] = static_cast<unsigned char>( diffuseTextureData[ i ] * clampedNdotL );
		outputImageData[ i + 1 ] = static_cast<unsigned char>( diffuseTextureData[ i+1 ] * clampedNdotL );
		outputImageData[ i + 2 ] = static_cast<unsigned char>( diffuseTextureData[ i+2 ] * clampedNdotL );
	}

	//Now output result to screen:

	// Enable texturing
	glEnable( GL_TEXTURE_2D );

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	if (g_outputTextureID == 0) glGenTextures( 1, (GLuint*)&g_outputTextureID ); //Only called once.

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture( GL_TEXTURE_2D, g_outputTextureID );

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP ); // one of: GL_CLAMP or GL_REPEAT
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP ); // one of: GL_CLAMP or GL_REPEAT

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );  // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	GLenum bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if ( numComponents == 3 )
		bufferFormat = GL_RGB;

	// #FIXME: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	//Called glTexImage2D(...) here! THIS IS WHERE MEMORY ACCESS ERROR OCCURS WHEN TRYING TO READ OUTPUT IMAGE DATA ARRAY!

	//g_outputImageData = nullptr; //Note if this is uncommented the memory error won't happen in glTexImage2D (but seems to skip rendering the texture altogether as a result).

	glTexImage2D(			// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,		// Creating this as a 2d texture
		0,					// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,		// Type of texel format we want OpenGL to use for this texture internally on the video card
		outputTextureSize.x,		// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		outputTextureSize.y,		// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,					// Border size, in texels (must be 0 or 1)
		bufferFormat,		// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,	// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		outputImageData );	// Location of the actual pixel data bytes/buffer



	glDisable( GL_TEXTURE_2D );

	stbi_write_png( "Data/Images/outputImage.png", outputTextureSize.x, outputTextureSize.y, numComponents, outputImageData, 0 );

	stbi_image_free( outputImageData );
	stbi_image_free( diffuseTextureData );
	stbi_image_free( bumpTextureData );
}

//-----------------------------------------------------------------------------------------------
void Update( )
{
	static double s_timeLastFrameBegan = GetCurrentTimeSeconds( ); //Called only once due static.
	double timeThisFrameBegan = GetCurrentTimeSeconds( );
	float deltaSeconds = static_cast<float>( timeThisFrameBegan - s_timeLastFrameBegan );
	s_timeLastFrameBegan = timeThisFrameBegan;

	g_theGame->Update( deltaSeconds ); //Can pass in 0 to freeze, or others to rewind/slow/etc.

	//Note below code is not framerate-corrected.
	static float s_lightDirX = 255.f / 2.f;
	static float s_lightDirY = 255.f / 2.f;
	static float s_lightDirZ = 255.f / 2.f;
	const float lightAnimStep = 255.f / 5.f;

	if ( g_theApp->isKeyDown( VK_SPACE ) && g_theApp->WasKeyJustPressed( VK_SPACE ) )
	{
		CalculateDOT3(s_lightDirX, s_lightDirY, s_lightDirZ);
	}
	//---------------------------------------------------------------------
	if ( g_theApp->isKeyDown( 'A' ) && g_theApp->WasKeyJustPressed( 'A' ) )
	{
		s_lightDirX -= lightAnimStep;
		ClampFloat( s_lightDirX, 0.f, 255.f );
	}
	if ( g_theApp->isKeyDown( 'D' ) && g_theApp->WasKeyJustPressed( 'D' ) )
	{
		s_lightDirX += lightAnimStep;
		ClampFloat( s_lightDirX, 0.f, 255.f );
	}
	//---------------------------------------------------------------------
	if ( g_theApp->isKeyDown( 'S' ) && g_theApp->WasKeyJustPressed( 'S' ) )
	{
		s_lightDirY -= lightAnimStep;
		ClampFloat( s_lightDirY, 0.f, 255.f );
	}
	if ( g_theApp->isKeyDown( 'W' ) && g_theApp->WasKeyJustPressed( 'W' ) )
	{
		s_lightDirY += lightAnimStep;
		ClampFloat( s_lightDirY, 0.f, 255.f );
	}
	//---------------------------------------------------------------------
	if ( g_theApp->isKeyDown( 'Q' ) && g_theApp->WasKeyJustPressed( 'Q' ) )
	{
		s_lightDirZ -= lightAnimStep;
		ClampFloat( s_lightDirZ, 0.f, 255.f );
	}
	if ( g_theApp->isKeyDown( 'E' ) && g_theApp->WasKeyJustPressed( 'E' ) )
	{
		s_lightDirZ += lightAnimStep;
		ClampFloat( s_lightDirZ, 0.f, 255.f );
	}
}


//-----------------------------------------------------------------------------------------------
void Render( )
{
	g_theRenderer->ClearScreenToColor( 0.f, 0.f, 0.f );
	g_theGame->Render( );
	g_theRenderer->DrawTexturedAABB(
		AABB2( 100.f, 100.f, 500.f, 500.f ),
		*Texture::CreateOrGetTexture( "Data/Images/diffuseTexture.png" ),
		Vector2( 0.f, 0.f ),
		Vector2( 1.f, 1.f )
		);

	g_theRenderer->DrawTexturedAABB(
		AABB2( 600.f, 100.f, 1000.f, 500.f ),
		*Texture::CreateOrGetTexture( "Data/Images/bumpTexture.png" ),
		Vector2( 0.f, 0.f ),
		Vector2( 1.f, 1.f )
		);


	AABB2 outputTexBounds = AABB2( 1100.f, 100.f, 1500.f, 500.f );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, g_outputTextureID );
	glBegin( GL_QUADS );
	{
		glTexCoord2f( 0.f, 1.f );
		glVertex2f( outputTexBounds.mins.x, outputTexBounds.mins.y );

		glTexCoord2f( 1.f, 1.f );
		glVertex2f( outputTexBounds.maxs.x, outputTexBounds.mins.y );

		glTexCoord2f( 1.f, 0.f );
		glVertex2f( outputTexBounds.maxs.x, outputTexBounds.maxs.y );

		glTexCoord2f( 0.f, 0.f );
		glVertex2f( outputTexBounds.mins.x, outputTexBounds.maxs.y );
	}
	glEnd();
	glDisable( GL_TEXTURE_2D );
	SwapBuffers( g_displayDeviceContext );
}

//-----------------------------------------------------------------------------------------------
void RunFrame( )
{
	g_theApp->Update( ); //Resets keys, putting this here validates WasKeyJustChanged.
	RunMessagePump( ); //Can rename this to just doInput, because it handles updating key callback.
	Update( );
	Render( );
}


//-----------------------------------------------------------------------------------------------
void Initialize( HINSTANCE applicationInstanceHandle )
{
	srand( static_cast<unsigned int>( time( NULL ) ) );
	CreateOpenGLWindow( applicationInstanceHandle );
	g_theApp = new TheApp( VIEW_RIGHT, VIEW_TOP );
	g_theGame = new TheGame( );
	g_theRenderer = new TheRenderer( );
}


//-----------------------------------------------------------------------------------------------
void Shutdown( )
{
	delete g_theApp;
	delete g_theGame;
	delete g_theRenderer;

	g_theApp = nullptr;
	g_theGame = nullptr;
	g_theRenderer = nullptr;
}


//-----------------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int )
{
	UNUSED( commandLineString );
	Initialize( applicationInstanceHandle );

	while ( !g_isQuitting )
	{
		RunFrame( );
	}

	Shutdown( );
	return 0;
}