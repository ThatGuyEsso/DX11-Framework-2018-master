#pragma once
#include "Matrix.h"
#include "Vector3D.h"
#include <cmath>
#include <iostream>
#include <directxmath.h>
//typedef const DirectX::XMVECTOR FXMVECTOR;

//using namespace DirectX;
class Camera {
private:
	//Position variables
	Vector3D _eye; //world space position
	Vector3D _at; //looking at position
	Vector3D _up; //Up vector
	Vector3D _forward; //forward vector
	Vector3D _right;
	bool _isLookingForward;
	bool _hasControl = true;

	//Camera settings
	float _windowWidth;
	float _windowHeight;
	float _nearDepth;
	float _farDepth;
	float _maxZoomDistance=2.0f;
	float _rotationAmount = 20.0f;
	float _rotationSpeed = 1.1f;
	float _cameraStrafeAmount=0.05f;
	float _cameraForwardMoveAmount = 0.05f;
	float _cameraVerticalMoveAmount = 0.05f;
	float _zoomSpeed =0.025;
	//time
	float time;

	// attributes to hold the view and projection matrices which 

	// will be passed to the shader
	Matrix<float> _view = Matrix<float>(4, 4, 0.0f);
	Matrix<float> _projection = Matrix<float>(4, 4, 0.0f);
private:
	Vector3D CreateRotation(float angle,Vector3D rotAxis);

public:
	//Constructor and destructor for the camera

	Camera(Vector3D position, Vector3D at, Vector3D up, float
		windowWidth, float windowHeight, float
		nearDepth, float farDepth);
	~Camera()=default;

	// update function to make the current view and projection 
// matrices
	void Update(float deltaTime);
	Vector3D GetPoistion();
	Vector3D GetLookAt();
	Vector3D GetUP();
	Vector3D GetRight();
	Vector3D GetForward();
	bool GetMovingForward();
	void SetPosition(Vector3D newPosition);
	void SetLookAt(Vector3D newPoint);
	void SetUp(Vector3D newUp);
	void SetForward(Vector3D newForward);
	void ToggleCameraMode();
	//CameraMovement
	//Lookat camera
	void Zoom(bool zoomIn);
	void RotateYAxis(bool rotateRight);
	void RotateXAxis(bool rotateUp);

	//Lookforward
	void Strafe(bool strafeRight);
	void Move(bool moveForward);
	void MoveOnY(bool up);
	void Roll (bool rollRight);



		// You will need functions to get the View, Projection and 
	// combined ViewProjection matrices
	Matrix<float> GetView();
	Matrix<float> GetProjection();
	Matrix<float> GetViewProj();
	DirectX::XMFLOAT4X4 GetViewXMMATRIX();
	DirectX::XMFLOAT4X4 GetProjectionXMMATRIX();

		// A function to reshape the camera volume if the window is 
	// resized.
	void Reshape(float windowWidth, float windowHeight, float
		nearDepth, float farDepth);
	
	void SetRotationAmount(float newAmount);
	void SetLookForward(bool lookForward);

	Matrix<float> ConvertToColumnVector(Vector3D vector);

	Vector3D ConvertToVector3D(Matrix<float> matrix3x1);
};
