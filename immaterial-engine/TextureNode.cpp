
#include <string.h>

#include "OpenGL.h"
#include "DEBUGGING.h"

#include "TextureNode.h"

TextureNode::TextureNode()
	: name(NOT_INITIALIZED), textureID(0), minFilter(GL_LINEAR), magFilter(GL_LINEAR), wrapMode(GL_CLAMP_TO_EDGE)
{
	// default constructor set everything to zero/default
	memset( this->assetName, 0, TEXT_ASSET_NAME_SIZE );
}

void TextureNode::set( const char * const inAssetName,
						GLuint inHashName,
						GLuint inTextureID,
						GLenum inMinFilter,
						GLenum inMagFilter,
						GLenum inWrapMode)
{
	if (strlen(inAssetName) < TEXT_ASSET_NAME_SIZE - 1)	{
		memcpy( this->assetName, inAssetName, strlen(inAssetName) );
		this->assetName[strlen(inAssetName)] = '\0';
	}
	else	{
		memcpy( this->assetName, inAssetName, TEXT_ASSET_NAME_SIZE - 1);
		this->assetName[TEXT_ASSET_NAME_SIZE - 1] = '\0';
	}
	this->name = inHashName;
	this->textureID = inTextureID;
	this->minFilter = inMinFilter;
	this->magFilter = inMagFilter;
	this->wrapMode = inWrapMode;
}