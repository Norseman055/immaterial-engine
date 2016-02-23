#include "TextureManager.h"

#include "DEBUGGING.h"
#include "TGAHeader.h"
#include "md5.h"

TextureManager::TextureManager() { }

// singleton
TextureManager* TextureManager::privGetInstance() {
	static TextureManager textMan;
	return &textMan;
}

void TextureManager::AddTexture( char * const inAssetName, const TextureName inName ) {
	GLuint textureID;
	auto pTextureID = &textureID;

	// load texture to gpu, get ID from gpu, add to list
	privGetInstance()->privLoadTexture( inAssetName, pTextureID );
	Add( new TextureNode( inAssetName, new Texture( inName, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE ) ) );
}

// for use when loading textures from .ctf (converted TGA file)
bool TextureManager::LoadTexture( char * const fileName, char * const textID ) {
	GLuint textureID;
	auto pTextureID = &textureID;

	// Load texture from buffer
	auto lTexture = getTGAFile( fileName );
	privGetInstance()->privLoadMyTexture( lTexture, pTextureID );

	// hash the string textID and store it as the textureID for the manager
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(textID), strlen( textID ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	// make a new node, set its values, add to list
	Add( new TextureNode( fileName, new Texture( hashID, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE ) ) );

	return true;
}

bool TextureManager::LoadBufferedTexture( unsigned char * const textBuff, tffInfo & inHdr ) {
	GLuint textureID;
	auto pTextureID = &textureID;

	privGetInstance()->privLoadMyTexture( textBuff, pTextureID );

	// hash the string inName and store it as the textureID for the manager
	MD5Output out;
	MD5Buffer( reinterpret_cast< unsigned char * >(inHdr.textName), strlen( inHdr.textName ), out );
	auto hashID = out.dWord_0 ^ out.dWord_1 ^ out.dWord_2 ^ out.dWord_3;

	// make a new node, set its values, add to list
	Add( new TextureNode( inHdr.textName, new Texture( hashID, textureID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE ) ) );

	return true;
}

GLuint TextureManager::GetTextureID( const GLuint inName ) {
	auto walker = static_cast< TextureNode * >(GetObjectList()->getRoot()->getChild());

	// if there is a texture to be asked for, return it
	if ( inName != NOT_INITIALIZED ) {
		// find node
		while ( walker != nullptr ) {
			if ( walker->getData()->textureName == inName ) {
				break;
			}

			walker = static_cast< TextureNode * >(walker->getSibling());
		}

		if ( walker == nullptr ) {
			return 0x0;
		}
	} else {
		// otherwise, return the dummy texture (first one ever loaded)
		while ( static_cast< TextureNode * >(walker->getSibling()) != nullptr ) {
			walker = static_cast< TextureNode * >(walker->getSibling());
		}
	}

	// return textureID stored in node, used by GPU
	return walker->getData()->textureID;
}

void TextureManager::DeleteTextures() {
	// delete textures from GPU
	auto walker = static_cast< TextureNode * >(GetObjectList()->getRoot()->getChild());
	while ( walker != nullptr ) {
		glDeleteTextures( 1, &walker->getData()->textureID );
		walker = static_cast< TextureNode * >(walker->getSibling());
	}
	// delete texture tree
	Cleanup();
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