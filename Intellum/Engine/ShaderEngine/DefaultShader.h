#pragma once

#include "IShaderType.h"

using namespace std;

class DefaultShader : public IShaderType
{
	struct LightBuffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
	
	void SetLightBuffer(unsigned bufferNumber) const;
public:
	DefaultShader(DirectX3D* _direct3D, Camera* camera, Light* light);
	~DefaultShader();

	void Initialise(HWND hwnd) override;
	void InitialiseShader(HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) override;
	void Shutdown() override;

	void SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray, int textureCount) override;

	void Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView** textureArray, int textureCount) override;
	void RenderShader(int indexCount) override;

	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) override;
};