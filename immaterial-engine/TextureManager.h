#pragma once

#include "TextureNode.h"
#include "GraphicsObjectFileHdr.h"

class TextureMan
{
public:
	static void AddTexture( const char * const inAssetName, const TextureName inName);
	static bool LoadTexture( const char * const fileName, const char * const TextID );
	static bool LoadBufferedTexture( const unsigned char * const textBuff, const tffInfo & info );
	static void DeleteTextures();
	static GLuint Find( const GLuint inName);

private:
	// singleton
	static TextureMan *privGetInstance();
	TextureMan();

	const void privLoadTexture( const char * const inAssetName, GLuint *&textureID );
	const void privLoadMyTexture( const unsigned char * const tgaData, GLuint *&textureID );
	bool privLoadTGATexture( const char *szFileName, GLenum inMinFilter, GLenum inMagFilter, GLenum inWrapMode );
	bool privLoadMyTGATexture( const unsigned char * const tgaData, GLenum inMinFilter, GLenum inMagFilter, GLenum inWrapMode );
	const void privAddToFront( TextureNodeLink *node, TextureNodeLink *&head );

private:
	TextureNodeLink *active;

};