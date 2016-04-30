#include "ArchiveManager.h"
#include "eat.h"
#include "PackageHeader.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "File.h"
#include "DEBUGGING.h"

void ArchiveMan::LoadArchive( const char * fileName ) {
	PackageHeader pHead;
	ChunkHeader cHead;
	FileHandle fh;
	FileError ferror;

	ferror = File::open( fh, fileName, FILE_READ );
	assert( ferror == FILE_SUCCESS );

	ferror = File::read( fh, &pHead, sizeof( PackageHeader ) );
	assert( ferror == FILE_SUCCESS );

	for ( auto i = 0; i < pHead.numChunks; i++ ) {
		ferror = File::read( fh, &cHead, sizeof( ChunkHeader ) );
		assert( ferror == FILE_SUCCESS );

		switch ( cHead.type ) {
			case VERTS_TYPE:	{
					auto buffer = new unsigned char[cHead.chunkSize];

					ferror = File::read( fh, buffer, cHead.chunkSize );
					assert( ferror == FILE_SUCCESS );

					ModelManager::LoadBufferedModel( buffer );
					break;
				}
			case TEXTURE_TYPE:	{
					tffInfo info;
					ferror = File::read( fh, &info, sizeof( tffInfo ) );
					assert( ferror == FILE_SUCCESS );

					auto texBuffer = new unsigned char[cHead.chunkSize - sizeof( tffInfo )];

					ferror = File::read( fh, texBuffer, cHead.chunkSize - sizeof( tffInfo ) );
					assert( ferror == FILE_SUCCESS );

					TextureManager::LoadBufferedTexture( texBuffer, info );
					break;
				}
			case ANIM_TYPE:	{
					AnimFileHdr aHdr;
					ferror = File::read( fh, &aHdr, sizeof( AnimFileHdr ) );
					assert( ferror == FILE_SUCCESS );

					auto animBuff = new unsigned char[cHead.chunkSize - sizeof( AnimFileHdr )];

					ferror = File::read( fh, animBuff, cHead.chunkSize - sizeof( AnimFileHdr ) );
					assert( ferror == FILE_SUCCESS );

					AnimationMan::LoadAnimationBuffer( animBuff, aHdr );
					break;
				}
			default:	{
					ferror = File::seek( fh, FILE_SEEK_CURRENT, cHead.chunkSize );
					assert( ferror == FILE_SUCCESS );
					break;
				}
		}
	}

	ferror = File::close( fh );
	assert( ferror == FILE_SUCCESS );
}