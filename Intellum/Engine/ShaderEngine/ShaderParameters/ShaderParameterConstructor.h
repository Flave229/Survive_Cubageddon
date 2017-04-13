#pragma once
#include "ShaderParameters.h"
#include "../../Objects/Appearance/GradientOverload.h"

class ColorShaderParameters;

class ShaderParameterConstructor
{
public:
	ShaderParameterConstructor();
	~ShaderParameterConstructor();
	static ShaderParameters ConstructDefaultBufferParameters(int bufferIndex);
	static ShaderParameters ConstructMatrixBufferParameters(int bufferIndex, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);
	static ShaderParameters ConstructColorOverloadBufferParameters(int bufferIndex, ColorShaderParameters colorOverload);
	static ShaderParameters ConstructTextureBufferParameters(int bufferIndex, int textureCount, bool lightMapEnabled, bool bumpMapEnabled);
	static ShaderParameters ConstructGradientOverloadBufferParameters(int bufferIndex, GradientOverload gradientOverload);
};
