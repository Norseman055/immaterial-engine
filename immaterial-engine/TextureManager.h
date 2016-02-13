#pragma once

#include "TextureNode.h"
#include "GraphicsObjectFileHdr.h"

class TextureMan {
public:
	static void AddTexture( char * const, const TextureName );
	static bool LoadTexture( char * const, char * const );
	static bool LoadBufferedTexture( unsigned char * const, tffInfo& );
	static void DeleteTextures();
	static GLuint Find( const GLuint inName );

private:
	// singleton
	static TextureMan *privGetInstance();
	TextureMan();

	void privLoadTexture( char * const inAssetName, GLuint *&textureID ) const;
	void privLoadMyTexture( unsigned char * const tgaData, GLuint *&textureID ) const;
	bool privLoadTGATexture( char * const szFileName, GLenum inMinFilter, GLenum inMagFilter, GLenum inWrapMode ) const;
	bool privLoadMyTGATexture( unsigned char * const tgaData, GLenum inMinFilter, GLenum inMagFilter, GLenum inWrapMode ) const;
	void privAddToFront( TextureNodeLink *node, TextureNodeLink *&head ) const;

private:
	TextureNodeLink *active;
};