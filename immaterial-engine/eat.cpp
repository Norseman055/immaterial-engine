#include <stdio.h>
#include <assert.h>
#include "eat.h"
#include "ChunkHeader.h"
#include "PackageHeader.h"
#include "File.h"
#include "DEBUGGING.h"

#define UNUSED_VAR(x) (void)x;

bool eat( const char * const inFileName, ChunkType type, const char * const chunkName, unsigned char *&chunkBuff, int &chunkSize)
{
	bool found = false;
	PackageHeader pHead;
	ChunkHeader cHead;

	// open file, read first chunk
	FileHandle fh;
	FileError ferror;

	ferror = File::open(fh, inFileName, FILE_READ);
	assert(ferror == FILE_SUCCESS);

	ferror = File::read(fh, &pHead, sizeof(PackageHeader));
	assert(ferror == FILE_SUCCESS);

	ferror = File::read(fh, &cHead, sizeof(ChunkHeader));
	assert(ferror == FILE_SUCCESS);

	// go through each chunk in the .spu file, checking if it matches type and name
	for (int i = 0; i < pHead.numChunks; i++)
	{
		if (cHead.type == type && strcmp(cHead.chunkName, chunkName) == 0)
		{
			found = true;
			break;
		}
		else
		{
			File::seek(fh, FILE_SEEK_CURRENT, cHead.chunkSize);
			File::read(fh, &cHead, sizeof(ChunkHeader));
		}
	}

	// at this point, file exists and found = true, or file does not exist and found = false.
	if (found)
	{
		chunkBuff = (unsigned char *)malloc(cHead.chunkSize);
		File::read(fh, chunkBuff, cHead.chunkSize);
		chunkSize = cHead.chunkSize;
	}

	ferror = File::close(fh);
	assert(ferror == FILE_SUCCESS);

	return found;
}

bool myEat(const char * const inFileName, ChunkType type, unsigned char *&chunkBuff, int &chunkSize)
{
	bool found = false;
	PackageHeader pHead;
	ChunkHeader cHead;

	// open file, read first chunk
	FileHandle fh;
	FileError ferror;

	ferror = File::open(fh, inFileName, FILE_READ);
	assert(ferror == FILE_SUCCESS);

	ferror = File::read(fh, &pHead, sizeof(PackageHeader));
	assert(ferror == FILE_SUCCESS);

	ferror = File::read(fh, &cHead, sizeof(ChunkHeader));
	assert(ferror == FILE_SUCCESS);

	// go through each chunk in the .spu file, checking if it matches type and name
	for (int i = 0; i < pHead.numChunks; i++)
	{
		if (cHead.type == type)
		{
			found = true;
			break;
		}
		else
		{
			File::seek(fh, FILE_SEEK_CURRENT, cHead.chunkSize);
			File::read(fh, &cHead, sizeof(ChunkHeader));
		}
	}

	// at this point, file exists and found = true, or file does not exist and found = false.
	if (found)
	{
		chunkBuff = (unsigned char *)malloc(cHead.chunkSize);
		File::read(fh, chunkBuff, cHead.chunkSize);
		chunkSize = cHead.chunkSize;
	}

	ferror = File::close(fh);
	assert(ferror == FILE_SUCCESS);

	return found;
}