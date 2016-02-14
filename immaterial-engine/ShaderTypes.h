#pragma once

enum ShaderType {
	Shader_Init,
	Shader_Texture_NoLights = Shader_Init,
	Shader_Texture_PointLights,
	Shader_NoTexture_PointLights,
	Shader_NoTexture_NoLights,
	Shader_Wireframe,
	Shader_End
};