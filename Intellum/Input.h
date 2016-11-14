#ifndef _INPUT_H_
#define _INPUT_H_
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <DirectXMath.h>

using namespace DirectX;

class Input
{
private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	unsigned char _keyboardState[256];
	DIMOUSESTATE _mouseState;

	int _screenWidth;
	int _screenHeight;

	int _mouseX;
	int _mouseY;

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

public:
	Input();
	Input(const Input& other);
	~Input();

	bool Initialise(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void MapMouseLocationInto(XMFLOAT2& point);
};

#endif