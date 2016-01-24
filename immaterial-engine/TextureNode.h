#ifndef TEXTURE_NODE_H
#define TEXTURE_NODE_H

#include "OpenGL.h"

#define TEXT_ASSET_NAME_SIZE 32

enum TextureName
{
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

class TextureNodeLink
{

public:
	TextureNodeLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	TextureNodeLink *next;
	TextureNodeLink *prev;
};

class TextureNode : public TextureNodeLink
{
public:
	TextureNode();
	void set( const char * const inAssetName,
				GLuint inHashName,
				GLuint inTextureID,
				GLenum inMinFilter,
				GLenum inMagFilter,
				GLenum inWrapMode);

private:
	char assetName[TEXT_ASSET_NAME_SIZE];

public:
	GLuint name;
	GLuint textureID;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapMode;
};

#endif