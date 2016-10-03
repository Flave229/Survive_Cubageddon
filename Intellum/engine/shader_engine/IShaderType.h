#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "../../Light.h"

using namespace DirectX;

public class IShaderType
{
public:
	virtual ~IShaderType() {};

	virtual bool Initialise(ID3D11Device* device, HWND hwnd) = 0;
	virtual bool InitialiseShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename) = 0;
	virtual void Shutdown() = 0;

	virtual bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light) = 0;

	virtual bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light) = 0;

	virtual void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount) = 0;
	
	virtual void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName) = 0;
};
