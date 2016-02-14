#pragma once

#include "GraphicsObject.h"

// Null object, this object does nothing - used for the root tree node

class NullObject : public GraphicsObject {
public:
	explicit NullObject( const char * const _Name );

	// must define, base class has abstract methods
	void transform( void ) override;
	void draw( void ) override;
	void setRenderState( void ) override;
	void checkCulling( void ) override;
	Vect getStartPos( void ) override;
};