#pragma once

#include "ChunkHeader.h"

bool eat( const char * const inName, ChunkType type, const char * const chunkName, unsigned char *&chunkBuff, int &chunkSize );
bool myEat( const char * const inName, ChunkType type, unsigned char *&chunkBuff, int &chunkSize );