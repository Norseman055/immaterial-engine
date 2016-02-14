#pragma once

#include <GLTools.h>
#include <GLBatch.h>

#define FREEGLUT_STATIC
#include <GL/GL.h>
#include <GL/glut.h>

// When enabling warning level 4, we get warning C4505: unreferenced local function has been removed
// This pragma kills this needless warning
#pragma warning(disable: 4505)