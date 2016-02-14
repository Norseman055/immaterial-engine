#pragma once

#include <malloc.h>

class Align16 {
public:
	void *operator new(size_t size);
	void *operator new[]( size_t size );

	void operator delete(void *p);
	void operator delete[]( void *p );
};

inline void* Align16::operator new(size_t size) {
	return _aligned_malloc( size, 16 );
}

inline void* Align16::operator new [] ( size_t size ) {
	return _aligned_malloc( size, 16 );
}

inline void Align16::operator delete(void* p) {
	_aligned_free( p );
}

inline void Align16::operator delete [] ( void* p ) {
	_aligned_free( p );
}