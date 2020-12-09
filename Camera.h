#pragma once
#include "Matrix.h"
#include "Vector3D.h"
#include <cmath>
#include <iostream>
#include <directxmath.h>
#include "GameObject.h"

#define pathSpeedIncreateRate  0.000001f;
#define pathSpeedIncrement   0.00001f;
#define maxPathSpeed  0.0005f;
#define minPathSpeed  0.0005f;
//using namespace DirectX;
class Camera {
private:
	//Position variables
	Vector3D _eye; //world space position
	Vector3D _at; //looking at position
	Vector3D _up; //Up vector
	Vector3D _forward;
	Vector3D _rotation;
	Vector3D defForward = Vector3D(0.0f, 0.0f, 1.0f); //forward vector
	
	GameObject* targetGameObject = nullptr;


	std::vector<Vector3D> pathPoints;
	bool _isLookingForward;
	bool _hasControl = true;
	bool _isFollowCam = true;
	//Camera settings
	float _windowWidth;
	float _windowHeight;
	float _nearDepth;
	float _farDepth;
	float _maxZoomDistance=2.0f;

	float _rotationSpeed= 0.001f;
	float _cameraStrafeAmount=0.05f;
	float _cameraForwardMoveAmount = 0.05f;
	float _cameraVerticalMoveAmount = 0.05f;
	float _zoomSpeed =0.025;
	

	//time
	float time;
	float pathSpeed = minPathSpeed;
	float timeToPathSpeedIncrease = pathSpeedIncreateRate;

	bool followPath =false;
	int currPathIndex = 0;
	// attributes to hold the view and projection matrices which 

	// will be passed to the shader
	Matrix<float> _view = Matrix<float>(4, 4, 0.0f);
	Matrix<float> _projection = Matrix<float>(4, 4, 0.0f);
private:
	Vector3D CreateRotation(float angle,Vector3D rotAxis);
	void UpdateTargetLookAt();
	Matrix<float> ConvertToColumnVector(Vector3D vector);
	Vector3D ConvertToVector3D(Matrix<float> matrix3x1);
	void FollowPath(float t);
	void StopFollowingPath();

	void CleanUp();
public:
	//Constructor and destructor for the camera

	Camera(Vector3D position, Vector3D at, Vector3D up, float
		windowWidth, float windowHeight, float
		nearDepth, float farDepth);
	~Camera();

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
	bool GetLookIsLookingForward() { return _isLookingForward; }
	void SetLookAt(Vector3D newPoint);
	void SetUp(Vector3D newUp);
	void SetForward(Vector3D newForward);
	void ToggleCameraMode();
	//CameraMovement
	//Lookat camera
	void Zoom(bool zoomIn);
	void RotateYAxis(bool rotateRight);
	void RotateXAxis(bool rotateUp);
	void adjustPitch(bool isLookingUp);
	void adjustYaw(bool isLookingRight);
	//Lookforward
	void Strafe(bool strafeRight);
	void Move(bool moveForward);
	void MoveOnY(bool up);
	void SetLookAtGameObject(GameObject* target);
	void ToggleFollow(bool isFollowing);

	void BeginFollowingPath();
	void SetPathSpeed(float newPathSpeed);



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
	
	void SetRotationSpeed(float newAmount);
	void SetLookForward(bool lookForward);
	Vector3D CalculateBezierPoint(float t, Vector3D p0, Vector3D p1, Vector3D p2, Vector3D p3);
	void AddPathPoint(Vector3D newPoint);
};
