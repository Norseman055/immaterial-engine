#pragma once

#include "PCSTree.h"
#include "MathEngine.h"

// forward declare (get into the habit, it's good to use)
class GraphicsObject;

// Singleton
class GraphicsObjMan {
public:
	static void AddObject( GraphicsObject* const );
	static void AddDebugObject( GraphicsObject* const );
	static void DrawObjects();
	static GraphicsObject* FindByLocation( Vect& );
	static GraphicsObject* GetFirstObj();
	static void DebugSwitch();
	static PCSTree *GetMainTree();
	static void DeleteGraphicsObjects();

private:

	// Methods
	GraphicsObjMan();
	static GraphicsObjMan *privGetInstance();
	const void privDrawObjectsDepthFirst( GraphicsObject* const ) const;
	GraphicsObject* privFindAtLocation( GraphicsObject* const, Vect& );

	// Data
	PCSTree goTree;
	// Culling objects
	PCSTree goCull;
	bool debugOn;
};