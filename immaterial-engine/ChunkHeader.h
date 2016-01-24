#pragma once

#define CHUNK_NAME_SIZE 20

enum ChunkType
{
	VERTS_TYPE = 0xA0000000,
	NORMS_TYPE,
	ANIM_TYPE,
	TEXTURE_TYPE,
	UV_TYPE
};

struct ChunkHeader
{
	ChunkType      type;
	char           chunkName[CHUNK_NAME_SIZE];
	int            chunkSize;
	unsigned int   hashNum;
};