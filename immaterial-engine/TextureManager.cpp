#include "DEBUGGING.h"
#include "File.h"

#include "TextureManager.h"
#include "GraphicsObjectFileHdr.h"
#include "TGAHeader.h"
#include "md5.h"

TextureMan::TextureMan()
	: active( nullptr ) { }

// singleton
TextureMan* TextureMan::privGetInstance() {
	static TextureMan textMan;
	return &textMan;
}

void TextureMan::AddTexture( const char * const inAssetName, const TextureName inName ) {
	GLuint textureID;
	auto pTextureID = &textureID;

	// get instance
	auto pTextMan = privGetInstance();

	// load texture to gpu, get ID from gpu
	pTextMan->privLoadTexture( inAssetName, pTextureID );

	// make a new node, set its values
	auto pNode = new TextureNode;
	pNode->set( inAssetName, inName, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );

	// add said node to front of active texture list
	pTextMan->privAddToFront( pNode, pTextMan->active );
}

// for use when loading textures from .ctf (converted TGA file)
bool TextureMan::LoadTexture( const char * const fileName, const char * const textID ) {
	GLuint textureID;
	auto pTextureID = &textureID;

	// get instance
	auto pTextMan = privGetInstance();

	auto lTexture = getTGAFile( fileName );
	pTextMan->privLoadMyTexture( lTexture, pTextureID );

	// hash the string textID and store it as the textureID for the manager
	MD5Output out;
	MD5Buffer( ( unsigned char * ) textID, strlen( textID ), out );
	GLuint hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	// make a new node, set its values
	auto pNode = new TextureNode;
	pNode->set( fileName, hashID, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );

	// add said node to front of active texture list
	pTextMan->privAddToFront( pNode, pTextMan->active );
	return true;
}

bool TextureMan::LoadBufferedTexture( const unsigned char * const textBuff, const tffInfo & inHdr ) {
	GLuint textureID;
	auto pTextureID = &textureID;

	// get instance
	auto pTextMan = privGetInstance();

	pTextMan->privLoadMyTexture( textBuff, pTextureID );

	// hash the string inName and store it as the textureID for the manager
	MD5Output out;
	MD5Buffer( ( unsigned char * ) inHdr.textName, strlen( inHdr.textName ), out );
	GLuint hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	// make a new node, set its values
	auto pNode = new TextureNode;
	pNode->set( inHdr.textName, hashID, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );

	// add said node to front of active texture list
	pTextMan->privAddToFront( pNode, pTextMan->active );
	return true;
}

GLuint TextureMan::Find( const GLuint inName ) {
	// get instance
	auto pTextMan = privGetInstance();

	auto walker = ( TextureNode * ) pTextMan->active;

	// if there is a texture to be asked for, return it
	if ( inName != NOT_INITIALIZED ) {
		// find node
		while ( walker != nullptr ) {
			if ( walker->name == inName ) {
				break;
			}

			walker = ( TextureNode * ) walker->next;
		}
	} else {
		// otherwise, return the dummy texture (first one ever loaded)
		while ( ( TextureNode * ) walker->next != nullptr ) {
			walker = ( TextureNode * ) walker->next;
		}
	}

	// return textureID stored in node, used by GPU
	return walker->textureID;
}

void TextureMan::DeleteTextures() {
	// delete textures from GPU
	auto walker = ( TextureNode * ) privGetInstance()->active;
	auto tmp = walker;
	while ( walker != nullptr ) {
		walker = ( TextureNode * ) walker->next;
		glDeleteTextures( 1, &tmp->textureID );
		delete tmp;
		tmp = walker;
	}
}

const void TextureMan::privAddToFront( TextureNodeLink *node, TextureNodeLink *&head ) {
	assert( node != 0 );

	// empty list
	if ( head == 0 ) {
		head = node;
	} else {
		// non-empty list, add to front
		node->next = head;
		head->prev = node;
		head = node;
	}
}

const void TextureMan::privLoadTexture( const char * const inAssetName, GLuint *&textureID ) {
	// get handle to texture, bind it, then load it.
	glGenTextures( 1, textureID );
	glBindTexture( GL_TEXTURE_2D, *textureID );
	this->privLoadTGATexture( inAssetName, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );
}

const void TextureMan::privLoadMyTexture( const unsigned char * const tgaData, GLuint *&textureID ) {
	// get handle to texture, bind it, then load it.
	glGenTextures( 1, textureID );
	glBindTexture( GL_TEXTURE_2D, *textureID );
	this->privLoadMyTGATexture( tgaData, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );
}

// Load a TGA as a 2D Texture. Completely initialize the state
bool TextureMan::privLoadTGATexture( const char *szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode ) {
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	// Read the texture bits
	pBits = gltReadTGABits( szFileName, &nWidth, &nHeight, &nComponents, &eFormat );
	if ( pBits == NULL )
		return false;

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D( GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
				  eFormat, GL_UNSIGNED_BYTE, pBits );

	if ( minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		 minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		 minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		 minFilter == GL_NEAREST_MIPMAP_NEAREST )
		 glGenerateMipmap( GL_TEXTURE_2D );

	return true;
}

bool TextureMan::privLoadMyTGATexture( const unsigned char * const tgaData, GLenum minFilter, GLenum magFilter, GLenum wrapMode ) {
	// my TGA Function
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	pBits = myReadTGABits( tgaData, nWidth, nHeight, nComponents, eFormat );

	if ( pBits == nullptr ) {
		return false;
	}

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexImage2D( GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
				  eFormat, GL_UNSIGNED_BYTE, pBits );

	if ( minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		 minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		 minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		 minFilter == GL_NEAREST_MIPMAP_NEAREST )
		 glGenerateMipmap( GL_TEXTURE_2D );

	return true;
}