#pragma once
#include "Matrix.h"
#include "Vector3D.h"
#include <cmath>
#include <iostream>
#include <directxmath.h>
#include "GameObject.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include "vendor/imgui/imgui_impl_dx11.h"
#define pathSpeedIncreateRate  0.000001f;
#define pathSpeedIncrement   0.00001f;
#define maxPathSpeed  0.0005f;
#define minPathSpeed  0.0005f;
#define defaultFollowOffset  -6.0f;
#define defaultFollowHeight  3.0f;
//using namespace DirectX;
class Camera {

public:
	enum CameraMode
	{
		FirstPerson,
		ThirdPerson,
		Perspective
	};
private:
	//Position variables
	Vector3D _eye; //world space position
	Vector3D _at; //looking at position
	Vector3D _up; //Up vector
	Vector3D _forward;
	Vector3D _right;
	Vector3D _rotation;
	Vector3D defForward = Vector3D(0.0f, 0.0f, 1.0f); //forward vector
	Vector3D defRight= Vector3D(1.0f, 0.0f, 0.0f); //right vector
	GameObject* targetGameObject = nullptr;
	CameraMode currMode = CameraMode::Perspective;

	std::vector<Vector3D> pathPoints;
	std::string cameraName;
	bool _isLookingForward;
	bool _hasControl = true;
	bool _isFollowCam = true;
	//Camera settings 
	float _windowWidth;
	float _windowHeight;
	float _nearDepth;
	float _farDepth;
	float _maxZoomDistance=2.0f;
	float cameraSpeed = 0.01f;
	Vector3D followCamOffset;

	


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
	//utilities
	Matrix<float> ConvertToColumnVector(Vector3D vector);
	Vector3D ConvertToVector3D(Matrix<float> matrix3x1);
	XMVECTOR Vector3DToXMVector(Vector3D vector);
	Vector3D XMVectorToVector3D(XMVECTOR vector);
	Vector3D Camera::CreateRotation(float angle, Vector3D rotAxis);
	void EvaluateCameraMode();

	//update functions
	void UpdateTargetLookAt();
	void FollowPath(float t);

	void StopFollowingPath();

	void CleanUp();
public:
	//Constructor and destructor for the camera

	Camera(Vector3D position, Vector3D at, Vector3D up, float
		windowWidth, float windowHeight, float
		nearDepth, float farDepth);
	~Camera();

	void DisplayCameraGUI();
	void EndGUI();
	// update function to make the current view and projection 
// matrices
	//change camera modes
	void ToggleFollow(bool isFollowing);
	void ToggleCameraMode();
	void BeginFollowingPath();


	//Lookat camera controls
	void Zoom(bool zoomIn);
	void RotateYAxis(bool rotateRight);
	void RotateXAxis(bool rotateUp);
	void adjustPitch(bool isLookingUp);
	void adjustYaw(bool isLookingRight);

	//Foward looking movement
	void Strafe(bool strafeRight);
	void Move(bool moveForward);
	void MoveOnY(bool up);//move up

	//Setters
	void SetLookAtGameObject(GameObject* target);
	void SetPathSpeed(float newPathSpeed);
	void SetCameraSpeed(float newAmount);
	void SetLookForward(bool lookForward);
	void SetPosition(Vector3D newPosition);
	void SetLookAt(Vector3D newPoint);
	void SetUp(Vector3D newUp);
	void SetForward(Vector3D newForward);
	void SetCameraMode(CameraMode NewMode);
	void SetCameraName(std::string camName) { cameraName = camName; }
	//getters
	Vector3D GetPosition();
	Vector3D GetLookAt();
	Vector3D GetUP();
	Vector3D GetRight();
	Vector3D GetForward();

	bool GetLookIsLookingForward() { return _isLookingForward; }

	void Update(float deltaTime);

	//viewproj matrixies 
	Matrix<float> GetView();
	Matrix<float> GetProjection();
	Matrix<float> GetViewProj();
	DirectX::XMFLOAT4X4 GetViewXMMATRIX();
	DirectX::XMFLOAT4X4 GetProjectionXMMATRIX();


	//reset camera to fit viewport
	void Reshape(float windowWidth, float windowHeight, float
		nearDepth, float farDepth);
	
	//camera pathings

	void AddPathPoint(Vector3D newPoint);

};
