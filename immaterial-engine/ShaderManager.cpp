#include "ShaderManager.h"

GLShaderManager& ShaderManager::GetGLShaderManager() {
	return *privGetInstance()->shaderManager;
}

void ShaderManager::InitializeStockShaders() {
	privGetInstance()->shaderManager->InitializeStockShaders();
}

ShaderManager* ShaderManager::privGetInstance() {
	static ShaderManager instance;
	return &instance;
}

ShaderManager::ShaderManager() {
	shaderManager = new GLShaderManager();
}