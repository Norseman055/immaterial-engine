#pragma once

#include "OpenGL.h"

enum TextureName {
	STONES,
	RED_BRICK,
	DUCKWEED,
	ROCKS,
	SPACE_FRIGATE,
	WARBEAR,
	POLARBEAR,
	BUGGY_DIFF,
	BUGGY_NRM,
	BUGGY_SPEC,
	ASTROBOY,
	NOT_INITIALIZED
};

class Texture {
public:
	Texture( const GLuint inName, const GLuint inTexID, const GLenum inMinFilter, const GLenum inMagFilter, const GLenum inWrapMode )
		: textureName( inName ), textureID( inTexID ), minFilter( inMinFilter ), magFilter( inMagFilter ), wrapMode( inWrapMode ) { }

	GLuint textureName;
	GLuint textureID;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapMode;
};