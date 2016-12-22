#pragma once
//#include <string>
#include <map>

#include "Engine/Math/IntVector2.hpp"


//-----------------------------------------------------------------------------
class Texture
{
public:
	static Texture* GetTextureByName( const std::string& imageFilePath );
	static Texture* CreateOrGetTexture( const std::string& imageFilePath );
	unsigned int GetTextureID() const { return m_openglTextureID; }

private:
	Texture( const std::string& imageFilePath );
	static std::map<std::string, Texture*> s_textureRegistry;
	unsigned int m_openglTextureID;
	IntVector2 m_texelSize;
};