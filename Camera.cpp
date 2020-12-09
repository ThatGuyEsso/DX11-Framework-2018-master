#include "Camera.h"


Camera::Camera(Vector3D position, Vector3D at, Vector3D up, float windowWidth, float windowHeight, float nearDepth, float farDepth)
{
	_eye = position;
	_at = at;
	_up = up;
	_windowWidth = windowHeight;
	_windowHeight = windowHeight;
	_nearDepth= nearDepth;
	_farDepth=farDepth;
	_rotation = Vector3D();
	_forward = defForward;
	_isLookingForward = false;
	
	SetForward(Vector3D (0.0f,0.0f,1.0f));


}
Camera::~Camera()
{
	CleanUp();
}
//============Updaters=================
void Camera::Update(float deltaTime)
{	
	
	time = deltaTime;
	//Set up temporary matrix to store current view matrix in XM format so Direct X operations can be run on it.
	DirectX::XMFLOAT4X4 tempView;
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(_eye.show_X(), _eye.show_Y(), _eye.show_Z(), 0.0f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(_at.show_X(), _at.show_Y(), _at.show_Z(), 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(_up.show_X(), _up.show_Y(), _up.show_Z(), 0.0f);
	DirectX::XMVECTOR defaultForward = DirectX::XMVectorSet(defForward.show_X(), defForward.show_Y(), defForward.show_Z(), 0.0f);
	DirectX::XMVECTOR forward;
	XMMATRIX rotDirection = XMMatrixRotationRollPitchYaw(_rotation.show_X(), _rotation.show_Y(), 0.0f);
	forward = XMVector3TransformCoord(defaultForward, rotDirection);
	//store new view matrix

	if (followPath) {
		FollowPath(time);
	}
	//Update transfomation matrix depending on the camera is looking at or to
	if (_isLookingForward) {
		XMStoreFloat4x4(&tempView, DirectX::XMMatrixLookToLH(Eye, forward, Up));
	}
	else {

		if (_isFollowCam) {
			UpdateTargetLookAt();
		}
		XMStoreFloat4x4(&tempView, DirectX::XMMatrixLookAtLH(Eye, At, Up));
	}
	
	for (int i = 0; i < _view.get_rows(); i++) {
		for (int j = 0; j < _view.get_cols(); j++) {
			_view(i, j) = tempView.m[i][j];//pass in new values of matrix
		}
	}
	//Set up temporary matrix to store current view matrix in XM format so Direct X operations can be run on it.
	DirectX::XMFLOAT4X4 tempProject;
	//Store new projection matrix
	XMStoreFloat4x4(&tempProject, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, _windowWidth / _windowHeight, _nearDepth, _farDepth));

	for (int i = 0; i < _projection.get_rows(); i++) {
		for (int j = 0; j < _projection.get_cols(); j++) {
			_projection(i, j) = tempProject.m[i][j];//pass in new projection Matrix values
		}
	}

}
void Camera::Reshape(float windowWidth, float windowHeight, float nearDepth, float farDepth)
{
	_windowWidth = windowWidth;
	_windowHeight = windowHeight;
	_nearDepth = nearDepth;
	_farDepth = farDepth;
}
void Camera::SetRotationSpeed(float newAmount)
{
	_rotationSpeed = newAmount;
}
void Camera::SetLookForward(bool lookForward)
{
	_isLookingForward = lookForward;
}
Vector3D Camera::CalculateBezierPoint(float t, Vector3D p0, Vector3D p1, Vector3D p2, Vector3D p3)
{

	return Vector3D();
}
Matrix<float> Camera::ConvertToColumnVector(Vector3D vector)
{
	Matrix<float> column =  Matrix<float>(3, 1, 0.0f);

	column(0, 0) = vector.show_X();
	column(1, 0) = vector.show_Y();
	column(2, 0) = vector.show_Z();
	return column;
}

Vector3D Camera::ConvertToVector3D(Matrix<float> matrix3x1)
{
	Vector3D newVector(matrix3x1(0, 0), matrix3x1(1, 0), matrix3x1(2, 0));
	return newVector;
}

//====================================

//=============GETTERS=================
Vector3D Camera::GetPoistion()
{
	return _eye;
}

Vector3D Camera::GetLookAt()
{
	return _at;
}

Vector3D Camera::GetUP()
{
	return _up;	
}

Vector3D Camera::GetRight()
{
	return  Vector3D(1.0f, _eye.show_Y(), _eye.show_Z());
}


void Camera::ToggleCameraMode()
{
	_isLookingForward = !_isLookingForward;
}

Vector3D Camera::CreateRotation(float angle,Vector3D rotAxis)
{
	
	float rotationAngle = angle * DirectX::XM_PI / 180;
	DirectX::XMVECTOR axis = DirectX::XMVectorSet(rotAxis.show_X(), rotAxis.show_Y(), rotAxis.show_Z(), 0.0f);
	DirectX::XMVECTOR origin = DirectX::XMVectorSet(_eye.show_X(), _eye.show_Y(), _eye.show_Z(), 0.0f);
	DirectX::XMVECTOR quaternionVector = DirectX::XMQuaternionRotationAxis(axis, rotationAngle);
	DirectX::XMVECTOR XM_NewPos = DirectX::XMVector3Rotate(origin, quaternionVector);

	DirectX::XMFLOAT4 temp;
	DirectX::XMStoreFloat4(&temp, XM_NewPos);

	return Vector3D(temp.x, temp.y, temp.z);

}

void Camera::UpdateTargetLookAt()
{
	if (targetGameObject != nullptr) {
		_at = targetGameObject->GetPosition();
	}
	
}

void Camera::CleanUp()
{
	delete targetGameObject;
	targetGameObject = nullptr;
}

Matrix<float> Camera::GetView()
{
	return _view;
}

Matrix<float> Camera::GetProjection()
{
	return _projection;
}

Matrix<float> Camera::GetViewProj()
{
	return _view * _projection;
}

DirectX::XMFLOAT4X4 Camera::GetViewXMMATRIX()
{
	DirectX::XMFLOAT4X4 matrixConversion;
	for (int i = 0; i < _view.get_rows(); i++) {
		for (int j = 0; j < _view.get_cols(); j++) {
			matrixConversion.m[i][j] = _view(i,j);//pass in new projection Matrix values
		}
	}
	return matrixConversion;

}

DirectX::XMFLOAT4X4 Camera::GetProjectionXMMATRIX()
{
	DirectX::XMFLOAT4X4 matrixConversion;
	for (int i = 0; i < _projection.get_rows(); i++) {
		for (int j = 0; j < _projection.get_cols(); j++) {
			matrixConversion.m[i][j] = _projection(i, j);//pass in new projection Matrix values
		}
	}
	return matrixConversion;
}

//=====================================

Vector3D Camera::GetForward()
{
	return Vector3D(_eye.show_X(),_eye.show_Y(),1.0f);
}

bool Camera::GetMovingForward()
{
	return _isLookingForward;
}

//============SETTERS===================
void Camera::SetPosition(Vector3D newPosition)
{
	_eye = newPosition;
}

void Camera::SetLookAt(Vector3D newPoint)
{
	_at = newPoint;
}

void Camera::SetUp(Vector3D newUp)
{
	_up = newUp;
}

void Camera::SetForward(Vector3D newForward)
{
	_forward = newForward;
}

void Camera::Zoom(bool zoomIn) {
	if (_hasControl) {

		if (!_isLookingForward) {
			if (zoomIn) {

				if (_eye.distance(_at) > _maxZoomDistance) {
					Vector3D moveDir = _at - _eye;

					SetPosition(_eye +(moveDir.normalization()*time)*_zoomSpeed);
				}

			}
			else {
				Vector3D moveDir = _at - _eye;

				SetPosition(_eye - (moveDir.normalization()*time* _zoomSpeed));
			}
		}
	}
}

void Camera::RotateYAxis(bool rotateRight)
{

	if (_hasControl) {

		if (!_isLookingForward) {
			
			float rotDir =1.0f;
			if (rotateRight) {
				
				rotDir *= -1.0f;
			
			}
			
			SetPosition(CreateRotation(rotDir *time, Vector3D(0.0f, 1.0f, 0.0f)));
		}
	}
}

void Camera::RotateXAxis(bool rotateUp)
{
	if (_hasControl) {

		if (!_isLookingForward) {

			float rotDir = 1.0f;
			if (!rotateUp) {

				rotDir *= -1.0f;

			}

			SetPosition(CreateRotation(rotDir *time, Vector3D(1.0f, 0.0f, 0.0f)));

			
		}
	}
}

void Camera::adjustPitch(bool isLookingUp)
{
	if (_hasControl) {
		if (_isLookingForward) {
			float newPitch = 0;
if (isLookingUp) {
	newPitch = _rotation.show_X() + time * _rotationSpeed;
}
else {
	newPitch = _rotation.show_X() - time * _rotationSpeed;
}
_rotation = Vector3D(newPitch, _rotation.show_Y(), _rotation.show_Z());
		}
	}
}

void Camera::adjustYaw(bool isLookingRight)
{
	if (_hasControl) {
		if (_isLookingForward) {
			float newYaw = 0;
			if (isLookingRight) {
				newYaw = _rotation.show_Y() + time * _rotationSpeed;
			}
			else {
				newYaw = _rotation.show_Y() - time * _rotationSpeed;
			}
			_rotation = Vector3D(_rotation.show_X(), newYaw, _rotation.show_Z());
		}
	}
}

void Camera::Strafe(bool strafeRight)
{
	if (_hasControl) {
		if (_isLookingForward) {
			if (strafeRight) {
				Vector3D moveDir = Vector3D(_eye.show_X() + _cameraStrafeAmount * time, _eye.show_Y(), _eye.show_Z());
				SetPosition(moveDir);
			}
			else {
				Vector3D moveDir = Vector3D(_eye.show_X() - _cameraStrafeAmount * time, _eye.show_Y(), _eye.show_Z());
				SetPosition(moveDir);
			}
		}
	}
}

void Camera::Move(bool moveForward)
{
	if (_hasControl) {
		if (_isLookingForward) {
			if (moveForward) {
				Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y(), _eye.show_Z() + _cameraStrafeAmount * time);
				SetPosition(moveDir);
			}
			else {
				Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y(), _eye.show_Z() - _cameraStrafeAmount * time);
				SetPosition(moveDir);
			}
		}
	}
}

void Camera::MoveOnY(bool up)
{
	if (_hasControl) {


		if (up) {
			Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y() + (time * _cameraVerticalMoveAmount), _eye.show_Z());
			SetPosition(moveDir);
		}
		else {
			Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y() - (time * _cameraVerticalMoveAmount), _eye.show_Z());
			SetPosition(moveDir);
		}

	}
}

void Camera::SetLookAtGameObject(GameObject* target)
{
	targetGameObject = target;
}

void Camera::ToggleFollow(bool isFollowing)
{
	_isFollowCam = isFollowing;
}

void Camera::BeginFollowingPath()
{
	if (_hasControl&& !pathPoints.empty()) {
		_hasControl = false;
		followPath = true;

	}
}

void Camera::SetPathSpeed(float newPathSpeed)
{

	pathSpeed = newPathSpeed;
}

void Camera::FollowPath(float t)
{

	Vector3D dir = pathPoints[currPathIndex] - _eye;
	Vector3D dirNORM = dir.normalization();

			
		SetPosition(_eye + dirNORM* pathSpeed * time);

	if (_eye.distance(pathPoints[currPathIndex]) < 0.05 && _eye.distance(pathPoints[currPathIndex]) > -0.05){
		_eye = pathPoints[currPathIndex];
		currPathIndex++;
		if (currPathIndex >= pathPoints.size()) {
			currPathIndex = 0;
			StopFollowingPath();
		}
				
	}
	//if (timeToPathSpeedIncrease <= 0.0f) {
	//	pathSpeed += pathSpeedIncrement;
	//	float max = maxPathSpeed;
	//	if (pathSpeed >(max))  pathSpeed = max;
	//	timeToPathSpeedIncrease = pathSpeedIncreateRate;
	//	
	//}
	//else {
	//	timeToPathSpeedIncrease -= time;
	//}

	
}

void Camera::StopFollowingPath()
{
	_hasControl = true;
	followPath = false;
	pathSpeed = minPathSpeed;
}

void Camera::AddPathPoint(Vector3D newPoint)
{
	pathPoints.push_back(newPoint);
}




//====================================
