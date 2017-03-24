#pragma once
#include <d3d11.h>
#include "../../DirectX3D.h"
#include "../Components/AppearanceComponent.h"
#include "../Components/TransformComponent.h"

#include "ISystem.h"
#include "../../ShaderEngine/ConstantBuffers/MatrixBuffer.h"
#include "../../ShaderEngine/ConstantBuffers/CameraBuffer.h"
#include "../../ShaderEngine/ConstantBuffers/LightBuffer.h"
#include "../../ShaderEngine/ConstantBuffers/TextureBuffer.h"
#include "../../ShaderEngine/ShaderParameters/ShaderParameterConstructor.h"
#include <d3dcompiler.h>
#include <fstream>

class RenderSystem : public ISystem
{
private:
	DirectX3D* _direct3D;
	MatrixBuffer* _matrixBuffer;
	CameraBuffer* _cameraBuffer;
	LightBuffer* _lightBuffer;
	TextureBuffer* _textureBuffer;

	Camera* _camera;
	Light* _light;

	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11SamplerState* _sampleState;

	int _renderCount;
	
	void BuildBufferInformation(Entity* entity, AppearanceComponent* appearance) const;
	static vector<ID3D11ShaderResourceView*> ExtractResourceViewsFrom(vector<Texture*> textures);

	bool CheckIfInsideFrustrum(Entity* entity, TransformComponent* transform, AppearanceComponent* appearance);
public:
	RenderSystem(DirectX3D* direct3D, HWND hwnd, Camera* camera, Light* light);
	~RenderSystem() override = default;
	void Shutdown() override;

	void Update(vector<Entity*> entities, float delta) override;
	void Render(vector<Entity*> entities) override;

	int RenderCount() const;
};