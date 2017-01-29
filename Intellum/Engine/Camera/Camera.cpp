#include "Camera.h"

Camera::Camera(Frustrum* frustrum, Transform* transform, Input* input): _input(input), _frustrum(frustrum), _transform(transform)
{
	_transform->SetPosition(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Update(0.0f);
}

Camera::~Camera()
{
}

void Camera::Shutdown()
{
	if (_transform)
	{
		delete _transform;
		_transform = nullptr;
	}

	if (_frustrum)
	{
		delete _frustrum;
		_frustrum = nullptr;
	}
}

void Camera::Update(float delta)
{
	_transform->SetAngularVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
	_transform->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));

	if (_input->IsControlPressed(CAMERA_MOVE_LEFT))
	{
		_transform->AddVelocity(XMFLOAT3(-1.5f, 0.0f, 0.0f));
	}
	else if (_input->IsControlPressed(CAMERA_MOVE_RIGHT))
	{
		_transform->AddVelocity(XMFLOAT3(1.5f, 0.0f, 0.0f));
	}

	if (_input->IsControlPressed(CAMERA_MOVE_UP))
	{
		_transform->AddVelocity(XMFLOAT3(0.0f, 1.5f, 0.0f));
	}
	else if (_input->IsControlPressed(CAMERA_MOVE_DOWN))
	{
		_transform->AddVelocity(XMFLOAT3(0.0f, -1.5f, 0.0f));
	}

	if (_input->IsControlPressed(CAMERA_LOOK_LEFT))
	{
		_transform->AddAngularVelocity(XMFLOAT3(0.0f, -3.0f, 0.0f));
	}
	else if (_input->IsControlPressed(CAMERA_LOOK_RIGHT))
	{
		_transform->AddAngularVelocity(XMFLOAT3(0.0f, 3.0f, 0.0f));
	}

	if (_input->IsControlPressed(CAMERA_LOOK_UP))
	{
		_transform->AddAngularVelocity(XMFLOAT3(3.0f, 0.0f, 0.0f));
	}
	else if (_input->IsControlPressed(CAMERA_LOOK_DOWN))
	{
		_transform->AddAngularVelocity(XMFLOAT3(-3.0f, 0.0f, 0.0f));
	}

	_transform->Update(delta);

	XMFLOAT3 rotation = _transform->GetRotation();
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMVECTOR upVector = XMLoadFloat3(&up);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);

	XMVECTOR positionVector = XMLoadFloat3(&_transform->GetPosition());
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	XMFLOAT4X4 viewMatrix;
	XMStoreFloat4x4(&viewMatrix, _viewMatrix);
	_frustrum->ConstructFrustrum(viewMatrix, SCREEN_DEPTH);
}

Frustrum* Camera::GetFrustrum() const
{
	return _frustrum;
}

Transform* Camera::GetTransform() const
{
	return _transform;
}

XMMATRIX Camera::GetViewMatrix() const
{
	return _viewMatrix;
}