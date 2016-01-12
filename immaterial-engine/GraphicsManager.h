#ifndef GRAPHICS_OBJECT_MANAGER_H
#define GRAPHICS_OBJECT_MANAGER_H

#include "PCSTree.h"
#include "MathEngine.h"

// forward declare (get into the habit, it's good to use)
class GraphicsObject;

// Singleton
class GraphicsObjMan
{
public:
	static void addObject( GraphicsObject *p);
	static void addDebugObject( GraphicsObject *p);
	static void drawObjects( void );
	static GraphicsObject* FindByLocation( Vect &inStartPos );
	static GraphicsObject* GetFirstObj();
	static void DebugSwitch();
	static PCSTree *getMainTree();
	static void DeleteGraphicsObjects();

private:

	// Methods
	GraphicsObjMan();
	static GraphicsObjMan	*privGetInstance();
	void privDrawObjectsDepthFirst( GraphicsObject *node ) const;
	GraphicsObject* privFindAtLocation( GraphicsObject *node, Vect &inPos );

	// Data
	class PCSTree goTree;
	// Culling objects
	class PCSTree goCull;
	bool debugOn;
};
#endif
