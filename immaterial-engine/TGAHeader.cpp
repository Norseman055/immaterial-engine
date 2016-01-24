#include "TGAHeader.h"
#include "DEBUGGING.h"
#include "File.h"

#include "TextureManager.h"

GLbyte* myReadTGABits( const void * lTexture, GLint & nWidth, GLint & nHeight, GLint & nComponents, GLenum & eFormat ) {
	unsigned long lImageSize;
	short sDepth;
	GLbyte *pBits = nullptr;

	nWidth = 0;
	nHeight = 0;
	nComponents = GL_RGB;
	eFormat = GL_RGB;	// For some reason, lTexture changes immediately after this line of code. WHY?????

	auto tgaHeader = ( TGAHEADER * ) lTexture;
	auto textLoc = ( void * ) (( unsigned int ) lTexture + sizeof( TGAHEADER ));

	nWidth = tgaHeader->width;
	nHeight = tgaHeader->height;

	// 1, 3, 4 bytes per pixel are good. rest are bad. tell them they're bad, load null texture.
	if ( tgaHeader->BitsPerPixel == 8 || tgaHeader->BitsPerPixel == 24 || tgaHeader->BitsPerPixel == 32 ) {
		sDepth = tgaHeader->BitsPerPixel / 8;
		lImageSize = tgaHeader->width * tgaHeader->height * sDepth;
		pBits = ( GLbyte * ) malloc( lImageSize * sizeof( GLbyte ) );

		// get address just beyond TGAHEADER in lTexture
		memcpy( pBits, textLoc, lImageSize );

		switch ( sDepth ) {
			case 3:
				eFormat = GL_BGR;
				nComponents = GL_RGB;
				break;
			case 4:
				eFormat = GL_BGRA;
				nComponents = GL_RGBA;
				break;
			case 1:
				eFormat = GL_LUMINANCE;
				nComponents = GL_LUMINANCE;
				break;
			default:
				break;
		}
	}

	return pBits;
}

unsigned char* getTGAFile( const char * const fileName ) {
	FileHandle	fh;
	FileError	ferror;

	// open file, read header
	ferror = File::open( fh, fileName, FILE_READ );
	assert( ferror == FILE_SUCCESS );

	tffInfo textHdr;

	ferror = File::read( fh, &textHdr, sizeof( tffInfo ) );
	assert( ferror == FILE_SUCCESS );

	// get file size
	ferror = File::seek( fh, FILE_SEEK_END, 0 );
	assert( ferror == FILE_SUCCESS );

	int fSize;

	ferror = File::tell( fh, fSize );
	assert( ferror == FILE_SUCCESS );

	fSize -= sizeof( tffInfo );	// fSize currently has size of entire file, need to remove sizeof tffInfo struct for just the TGA file

	ferror = File::seek( fh, FILE_SEEK_BEGIN, sizeof( tffInfo ) );
	assert( ferror == FILE_SUCCESS );

	// create buffer for TGA file, read in the texture
	auto lTexture = ( unsigned char * ) malloc( fSize );

	ferror = File::read( fh, lTexture, fSize );
	assert( ferror == FILE_SUCCESS );

	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );

	return lTexture;
}