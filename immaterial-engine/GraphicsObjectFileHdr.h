#pragma once

#include <string.h>

#define OBJ_NAME_SIZE	32
#define TEXT_NAME_SIZE	32

struct tffInfo // texturefromfileInfo
{
	char		 textName[TEXT_NAME_SIZE];
	unsigned int textMinFilter;
	unsigned int textMagFilter;
	unsigned int textWrapMode;

	tffInfo()
		: textMinFilter( 0 ), textMagFilter( 0 ), textWrapMode( 0 ) { }
};

struct gObjFileHdr {
	// object name
	char         objName[OBJ_NAME_SIZE];

	// vertex buffer
	int          numVerts;
	int          vertBufferOffset;

	// trilist index
	int          numTriList;
	int          triListBufferOffset;

	// texture info
	int			 numTextures;
	tffInfo 	 textInfo;

	gObjFileHdr()
		: numVerts( 0 ), vertBufferOffset( 0 ), numTriList( 0 ), triListBufferOffset( 0 ) { }
};