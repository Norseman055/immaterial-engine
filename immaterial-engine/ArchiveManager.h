#pragma once

#include "DEBUGGING.h"

class ArchiveMan {
public:
	static void LoadArchive( const char * fileName );

private:
	ArchiveMan();
	static ArchiveMan * privGetInstance();
};