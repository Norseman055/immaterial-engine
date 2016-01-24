#pragma once

#include "GraphicsObject.h"

// Null object, this object does nothing - used for the root tree node

class NullObject : public GraphicsObject {
public:
	NullObject( const char * const _Name );

	// must define, base class has abstract methods
	void transform( void );
	void draw( void );
	void setRenderState( void );
	void checkCulling( void );
	Vect getStartPos( void );
};