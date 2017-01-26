#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "../Camera/Camera.h"
#include "../ShaderEngine/ShaderController.h"
#include "../ShaderEngine/IShaderType.h"
#include "../ShaderEngine/DefaultShader.h"
#include "Light.h"
#include "Bitmap.h"
#include "../../ErrorHandling/Exception.h"
#include "../DirectX3D.h"
#include "../FontEngine/FontEngine.h"
#include "../SystemMetrics/FramesPerSecond.h"
#include "../SystemMetrics/Cpu.h"
#include "../Object/Appearance.h"
#include "../Object/SceneObject.h"
#include "../Handlers/ObjectHandler.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
private:
	DirectX3D* _direct3D;

	FontEngine* _fontEngine;
	FramesPerSecond* _framesPerSecond;
	Cpu* _cpu;
	
	ShaderController* _shaderController;
	ObjectHandler* _objectHandler;

	Camera* _camera;
	Light* _light;
	Bitmap* _bitmap;

private:
	void Initialise(Box screenSize, HWND hwnd);

	bool Render(float delta, XMFLOAT2 mousePoint);
public:
	Graphics(Box screenSize, HWND hwnd, FramesPerSecond* framesPerSecond, Cpu* cpu);
	Graphics(const Graphics& other);
	~Graphics();

	void Shutdown();
	bool Frame(float delta, XMFLOAT2 mousePoint);
};

#endif