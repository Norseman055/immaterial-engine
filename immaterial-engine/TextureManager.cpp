#include "DEBUGGING.h"

#include "TextureManager.h"
#include "GraphicsObjectFileHdr.h"
#include "TGAHeader.h"
#include "md5.h"

TextureManager::TextureManager()
	: active( nullptr ) { }

// singleton
TextureManager* TextureManager::privGetInstance() {
	static TextureManager textMan;
	return &textMan;
}

void TextureManager::AddTexture( char * const inAssetName, const TextureName inName ) {
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
bool TextureManager::LoadTexture( char * const fileName, char * const textID ) {
	GLuint textureID;
	auto pTextureID = &textureID;

	// get instance
	auto pTextMan = privGetInstance();

	auto lTexture = getTGAFile( fileName );
	pTextMan->privLoadMyTexture( lTexture, pTextureID );

	// hash the string textID and store it as the textureID for the manager
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(textID), strlen( textID ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	// make a new node, set its values
	auto pNode = new TextureNode;
	pNode->set( fileName, hashID, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );

	// add said node to front of active texture list
	pTextMan->privAddToFront( pNode, pTextMan->active );
	return true;
}

bool TextureManager::LoadBufferedTexture( unsigned char * const textBuff, tffInfo & inHdr ) {
	GLuint textureID;
	auto pTextureID = &textureID;

	// get instance
	auto pTextMan = privGetInstance();

	pTextMan->privLoadMyTexture( textBuff, pTextureID );

	// hash the string inName and store it as the textureID for the manager
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(inHdr.textName), strlen( inHdr.textName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	// make a new node, set its values
	auto pNode = new TextureNode;
	pNode->set( inHdr.textName, hashID, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );

	// add said node to front of active texture list
	pTextMan->privAddToFront( pNode, pTextMan->active );
	return true;
}

GLuint TextureManager::Find( const GLuint inName ) {
	// get instance
	auto pTextMan = privGetInstance();

	auto walker = static_cast< TextureNode * >(pTextMan->active);

	// if there is a texture to be asked for, return it
	if ( inName != NOT_INITIALIZED ) {
		// find node
		while ( walker != nullptr ) {
			if ( walker->name == inName ) {
				break;
			}

			walker = static_cast< TextureNode * >(walker->next);
		}

		if ( walker == nullptr ) {
			return 0x0;
		}
	} else {
		// otherwise, return the dummy texture (first one ever loaded)
		while ( static_cast< TextureNode * >(walker->next) != nullptr ) {
			walker = static_cast< TextureNode * >(walker->next);
		}
	}

	// return textureID stored in node, used by GPU
	return walker->textureID;
}

void TextureManager::DeleteTextures() {
	// delete textures from GPU
	auto walker = static_cast< TextureNode * >(privGetInstance()->active);
	auto tmp = walker;
	while ( walker != nullptr ) {
		walker = static_cast< TextureNode * >(walker->next);
		glDeleteTextures( 1, &tmp->textureID );
		delete tmp;
		tmp = walker;
	}
}

void TextureManager::privAddToFront( TextureNodeLink *node, TextureNodeLink *&head ) const {
	assert( node );

	// empty list
	if ( head == nullptr ) {
		head = node;
	} else {
		// non-empty list, add to front
		node->next = head;
		head->prev = node;
		head = node;
	}
}

void TextureManager::privLoadTexture( char * const inAssetName, GLuint *&textureID ) const {
	// get handle to texture, bind it, then load it.
	glGenTextures( 1, textureID );
	glBindTexture( GL_TEXTURE_2D, *textureID );
	this->privLoadTGATexture( inAssetName, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );
}

void TextureManager::privLoadMyTexture( unsigned char * const tgaData, GLuint *&textureID ) const {
	// get handle to texture, bind it, then load it.
	glGenTextures( 1, textureID );
	glBindTexture( GL_TEXTURE_2D, *textureID );
	this->privLoadMyTGATexture( tgaData, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE );
}

// Load a TGA as a 2D Texture. Completely initialize the state
bool TextureManager::privLoadTGATexture( char * const szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode ) const {
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;

	// Read the texture bits
	pBits = gltReadTGABits( szFileName, &nWidth, &nHeight, &nComponents, &eFormat );
	if ( pBits == nullptr )
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

bool TextureManager::privLoadMyTGATexture( unsigned char * const tgaData, GLenum minFilter, GLenum magFilter, GLenum wrapMode ) const {
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