#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
#include "../Objects/Transform/Transform.h"
#include "Frustrum.h"
#include "../../Common/Constants.h"
#include "../Input/Input.h"

using namespace DirectX;

class Camera
{
private:
	Input* _input;

	Frustrum* _frustrum;
	Transform* _transform;

	XMMATRIX _viewMatrix2D;
	XMMATRIX _viewMatrix;

	void HandleRotationInput() const;
	void HandleMovementInput(XMFLOAT3 lookAt, XMFLOAT3 up) const;
public:
	Camera(Frustrum* frustrum, Transform* transform, Input* input);
	~Camera();

	void Shutdown();
	void Update(float delta);

	Frustrum* GetFrustrum() const;
	Transform* GetTransform() const;

	XMMATRIX GetViewMatrix() const;
};

#endif