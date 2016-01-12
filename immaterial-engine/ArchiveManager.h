
#ifndef ARCHIVE_MANAGER_H
#define ARCHIVE_MANAGER_H

#include "DEBUGGING.h"

class ArchiveMan
{
public:
	static void LoadArchive( const char * fileName );

private:
	ArchiveMan();
	static ArchiveMan * privGetInstance();
};

#endif