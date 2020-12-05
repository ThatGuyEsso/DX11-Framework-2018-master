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
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(_forward.show_X(), _forward.show_Y(), _forward.show_Z(), 0.0f);
	//store new view matrix

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
void Camera::SetRotationAmount(float newAmount)
{
	_rotationAmount = newAmount;
}
void Camera::SetLookForward(bool lookForward)
{
	_isLookingForward = lookForward;
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
			
			float rotDeg = _rotationAmount;
			if (rotateRight) {
				
				rotDeg *= -1;
			
			}
			
			SetPosition(CreateRotation(rotDeg *time, Vector3D(0.0f, 1.0f, 0.0f)));
		}
	}
}

void Camera::RotateXAxis(bool rotateUp)
{
	if (_hasControl) {

		if (!_isLookingForward) {

			float rotDeg = _rotationAmount;
			if (!rotateUp) {

				rotDeg *= -1;

			}

			SetPosition(CreateRotation(rotDeg*time, Vector3D(1.0f, 0.0f, 0.0f)));

			
		}
	}
}

void Camera::Strafe(bool strafeRight)
{
	if (_hasControl) {
		if (_isLookingForward) {
			if (strafeRight) {
				Vector3D moveDir = Vector3D(_eye.show_X() + _cameraStrafeAmount*time, _eye.show_Y(), _eye.show_Z());
				SetPosition(moveDir);
			}
			else {
				Vector3D moveDir = Vector3D(_eye.show_X() - _cameraStrafeAmount*time, _eye.show_Y(), _eye.show_Z());
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
				Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y(), _eye.show_Z() + _cameraStrafeAmount*time);
				SetPosition(moveDir);
			}
			else {
				Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y(), _eye.show_Z() - _cameraStrafeAmount*time);
				SetPosition(moveDir);
			}
		}
	}
}

void Camera::MoveOnY(bool up)
{
	if (_hasControl) {

			
			if (up) {
				Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y() + (time*_cameraVerticalMoveAmount), _eye.show_Z());
				SetPosition(moveDir);
			}
			else {
				Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y()- (time * _cameraVerticalMoveAmount), _eye.show_Z());
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




//====================================
