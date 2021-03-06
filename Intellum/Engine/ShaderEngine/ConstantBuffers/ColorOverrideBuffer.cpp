#include "ColorOverrideBuffer.h"

ColorOverrideBuffer::ColorOverrideBuffer(DirectX3D* direct3D) : IShaderBuffer(direct3D), _buffer(nullptr)
{
	ColorOverrideBuffer::Initialise();
}

void ColorOverrideBuffer::Shutdown()
{
	if (_buffer)
	{
		_buffer->Release();
		_buffer = nullptr;
	}
}

void ColorOverrideBuffer::Initialise()
{
	D3D11_BUFFER_DESC colorBufferDesc;
	colorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	colorBufferDesc.ByteWidth = sizeof(Buffer);
	colorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	colorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	colorBufferDesc.MiscFlags = 0;
	colorBufferDesc.StructureByteStride = 0;

	HRESULT result = _direct3D->GetDevice()->CreateBuffer(&colorBufferDesc, nullptr, &_buffer);
	if (FAILED(result)) throw Exception("Failed to create the buffer for the color description");
}

void ColorOverrideBuffer::SetShaderParameters(int bufferIndex, ShaderResources shaderResources)
{
	ColorShaderParameters colorOverload = shaderResources.ColorParameters;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map color buffer to the Device Context.");

	Buffer* colorDataPtr = static_cast<Buffer*>(mappedResource.pData);

	if (colorOverload.Enabled)
		colorDataPtr->colorOverloadEnabled = 1.0f;
	else
		colorDataPtr->colorOverloadEnabled = 0.0f;

	colorDataPtr->colorOverload = colorOverload.Color;
	colorDataPtr->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);

	_direct3D->GetDeviceContext()->Unmap(_buffer, 0);
	_direct3D->GetDeviceContext()->PSSetConstantBuffers(bufferIndex, 1, &_buffer);
}