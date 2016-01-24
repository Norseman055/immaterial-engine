#pragma once

#include "OpenGL.h"

#define TEXT_ASSET_NAME_SIZE 32

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

class TextureNodeLink {
public:
	TextureNodeLink()
		: next(nullptr), prev(nullptr)
	{ }

	TextureNodeLink *next;
	TextureNodeLink *prev;
};

class TextureNode : public TextureNodeLink {
public:
	TextureNode();
	const void set( const char * const inAssetName,
					const GLuint inHashName,
					const GLuint inTextureID,
					const GLenum inMinFilter,
					const GLenum inMagFilter,
					const GLenum inWrapMode );

private:
	char assetName[TEXT_ASSET_NAME_SIZE];

public:
	GLuint name;
	GLuint textureID;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapMode;
};