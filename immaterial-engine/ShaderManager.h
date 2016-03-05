#pragma once

#include "OpenGL.h"

class ShaderManager {
public:
	static GLShaderManager& GetGLShaderManager();
	static void InitializeStockShaders();

private:
	static ShaderManager* privGetInstance();
	ShaderManager();

	GLShaderManager shaderManager;
};