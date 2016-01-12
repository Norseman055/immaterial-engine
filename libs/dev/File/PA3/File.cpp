
#include <assert.h>
#include "File.h"

#define UNUSED(x) x


FileError File::open( FileHandle &fh, const char * const fileName, FileMode mode )
{
	assert(fh);
	assert(mode);
	FileError fE = (FileError)-1;

	switch(mode)
	{
		case FILE_READ:
			fh = CreateFile(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
		case FILE_WRITE:
			fh = CreateFile(fileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
		case FILE_READ_WRITE:
			fh = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
	}

	if (fh != INVALID_HANDLE_VALUE)
		fE = FILE_SUCCESS;
	else
		fE = FILE_OPEN_FAIL;

	return fE;
}

FileError File::close( const FileHandle fh )
{
	
	assert(fh);
	FileError fE = (FileError)-1;
	BOOL errorFlag = FALSE;

	errorFlag = CloseHandle(fh);

	if (errorFlag == TRUE)
		fE = FILE_SUCCESS;
	else
		fE = FILE_CLOSE_FAIL;
		
	return fE;
}

FileError File::write( FileHandle fh, const void * const buffer, const size_t inSize )
{
	assert(fh);
	FileError fE = (FileError)-1;
	DWORD pointer;
	BOOL errorFlag = FALSE;

	if (buffer != NULL)
	{
		errorFlag = WriteFile(fh, buffer, inSize, &pointer, NULL);
	}

	if (errorFlag == TRUE)
		fE = FILE_SUCCESS;
	else
		fE = FILE_WRITE_FAIL;

	return fE;
}

FileError File::read( FileHandle fh,  void * const buffer, const size_t inSize )
{
	assert(fh);
	FileError fE = (FileError)-1;
	DWORD pointer;
	BOOL errorFlag = FALSE;

	if (buffer != NULL)
	{
		errorFlag = ReadFile(fh, buffer, inSize, &pointer, NULL);
	}

	if (errorFlag == TRUE)
		fE = FILE_SUCCESS;
	else
		fE = FILE_READ_FAIL;

	return fE;
}

FileError File::seek( FileHandle fh, FileSeek seek, int offset )
{
	assert(fh);
	FileError fE = (FileError)-1;
	DWORD errorFlag = NO_ERROR;

	switch (seek)
	{
		case FILE_SEEK_BEGIN:
			errorFlag = SetFilePointer(fh, offset, NULL, FILE_BEGIN);
			break;
		case FILE_SEEK_CURRENT:
			errorFlag = SetFilePointer(fh, offset, NULL, FILE_CURRENT);
			break;
		case FILE_SEEK_END:
			errorFlag = SetFilePointer(fh, offset, NULL, FILE_END);
			break;
		default:
			break;
	}

	if (errorFlag == 0xffffffff && GetLastError() != NO_ERROR)
		fE = FILE_SEEK_FAIL;
	else
		fE = FILE_SUCCESS;

	return fE;
}

FileError File::tell( FileHandle fh, int &offset )
{	
	assert(fh);
	FileError fE = (FileError)-1;
	DWORD errorFlag = NO_ERROR;

	errorFlag = SetFilePointer(fh, 0, NULL, FILE_CURRENT);
	offset = SetFilePointer(fh, 0, NULL, FILE_CURRENT);

	if (errorFlag == 0xffffffff && GetLastError() != NO_ERROR)
		fE = FILE_TELL_FAIL;
	else
		fE = FILE_SUCCESS;
	
	return fE;
}

FileError File::flush( FileHandle fh )
{
	assert(fh);
	FileError fE = (FileError)-1;
	DWORD errorFlag = FALSE;

	errorFlag = FlushFileBuffers(fh);

	if (errorFlag == FALSE)
		fE = FILE_FLUSH_FAIL;
	else 
		fE = FILE_SUCCESS;

	return fE;
}