#include "Model.h"
#include "loaders/OBJLoader.h"

Model::Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext): _device(device), _deviceContext(deviceContext), _geometry(new Geometry), _texture(nullptr), _shader(new DefaultShader(device, deviceContext))
{
}

Model::Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext, IShaderType* shader) : _device(device), _deviceContext(deviceContext), _geometry(new Geometry), _texture(nullptr), _shader(shader)
{
	
}

Model::Model(const Model& other) : _device(other._device), _deviceContext(other._deviceContext), _geometry(other._geometry), _texture(other._texture), _shader(other._shader)
{
}

Model::~Model()
{
}

bool Model::Initialise(char* textureFilename, char* modelFilename)
{
	bool result;

	result = LoadModel(modelFilename);
	if (!result) return false;

	result = LoadTexture(textureFilename);
	if (!result) return false;

	return true;
}

void Model::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

void Model::Render(int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 cameraPosition, Light* light)
{
	RenderBuffers();

	_shader->Render(indexCount, worldMatrix, viewMatrix, projectionMatrix, texture, cameraPosition, light);
}

int Model::GetIndexCount()
{
	return _geometry->IndexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return _texture->GetTexture();
}

void Model::ShutdownBuffers()
{
	if(_geometry)
	{
		_geometry->Shutdown();
		delete _geometry;
		_geometry = nullptr;
	}
}

void Model::RenderBuffers()
{
	_deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &_geometry->VBStride, &_geometry->VBOffset);
	_deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Model::LoadTexture(char* filename)
{
	bool result;

	_texture = new Texture(_device, _deviceContext);
	if (!_texture) return false;

	result = _texture->Initialise(filename);
	if (!result) return false;

	return true;
}

void Model::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = nullptr;
	}
}

bool Model::LoadModel(char* fileName)
{
	*_geometry = OBJLoader::Load(fileName, _device);
	if (!_geometry) return false;

	return true;
}