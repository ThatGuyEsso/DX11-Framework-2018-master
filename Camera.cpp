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
	float followHeight = defaultFollowHeight;
	float followOffset = defaultFollowOffset
	followCamOffset = Vector3D(0.0f, followHeight, followOffset);
	SetForward(Vector3D (0.0f,0.0f,1.0f));
	EvaluateCameraMode();

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
	DirectX::XMVECTOR Eye = Vector3DToXMVector(_eye);
	DirectX::XMVECTOR At = Vector3DToXMVector(_at);
	DirectX::XMVECTOR Up = Vector3DToXMVector(_up);

	//create vectors ready to addd rotation transformations to
	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR right;
	//Update rotation based on camera roation
	XMMATRIX rotDirection = XMMatrixRotationRollPitchYaw(_rotation.show_X(), _rotation.show_Y(), 0.0f);

	//store new roation matrix in the z and x axis
	forward = XMVector3TransformCoord(Vector3DToXMVector(defForward), rotDirection);
	right = XMVector3TransformCoord(Vector3DToXMVector(defRight), rotDirection);
	
	//reupload new forward information to vector3d struct
	_forward = XMVectorToVector3D(forward);
	_right = XMVectorToVector3D(right);

	//Update camera transformation matrix based on camera mode
	switch (currMode) {
		case CameraMode::FirstPerson:
			XMStoreFloat4x4(&tempView, DirectX::XMMatrixLookToLH(Eye, forward, Up));

		break;
		case CameraMode::Perspective:
			XMStoreFloat4x4(&tempView, DirectX::XMMatrixLookAtLH(Eye, At, Up));
			//store new view matrix
			if (followPath) {
				FollowPath(time);
			}
			break;
		case CameraMode::ThirdPerson:
			if(targetGameObject!=nullptr){
				UpdateTargetLookAt();
				SetPosition(targetGameObject->GetPosition() + followCamOffset);
				XMStoreFloat4x4(&tempView, DirectX::XMMatrixLookToLH(Eye, forward, Up));

			}
			break;
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
void Camera::UpdateTargetLookAt()
{
	if (targetGameObject != nullptr) {
		_at = targetGameObject->GetPosition();
	}
	
}
void Camera::SetCameraSpeed(float newAmount)
{
	cameraSpeed = newAmount;
}
void Camera::SetLookForward(bool lookForward)
{
	_isLookingForward = lookForward;
}
//converts a vector to a colum matrix structure
Matrix<float> Camera::ConvertToColumnVector(Vector3D vector)
{
	//create matrix
	Matrix<float> column =  Matrix<float>(3, 1, 0.0f);
	//set column matrix data
	column(0, 0) = vector.show_X();
	column(1, 0) = vector.show_Y();
	column(2, 0) = vector.show_Z();
	return column;
}

//convert column vector back to vector3D class
Vector3D Camera::ConvertToVector3D(Matrix<float> matrix3x1)
{
	Vector3D newVector(matrix3x1(0, 0), matrix3x1(1, 0), matrix3x1(2, 0));
	return newVector;
}

//convert vector3D to XMVector
XMVECTOR Camera::Vector3DToXMVector(Vector3D vector)
{
	XMVECTOR newXMVector = XMVectorSet(vector.show_X(), vector.show_Y(), vector.show_Z(),0.0f);
	return newXMVector;
}

//convert XM>Vector to Vector3D
Vector3D Camera::XMVectorToVector3D(XMVECTOR vector)
{
	//create float3 to store xmvector
	XMFLOAT4 float4;
	XMStoreFloat4(&float4, vector);
	Vector3D newVector = Vector3D(float4.x, float4.y, float4.z);

	//return new vector
	return newVector;
}

//====================================

//=============GETTERS=================
Vector3D Camera::GetPosition()
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
	return  _right;
}

//cycles between camera modes
void Camera::ToggleCameraMode()
{
	//get current enum value
	int mode = (int)currMode;
	//check for overflow
	if (mode >= 2) {
		mode = 0;//if overflow reset it
	}
	else {
		//Not overflowing increment mode
		mode++;
	}
	//set new camera mode
	currMode = (CameraMode)mode;
	EvaluateCameraMode();

}

//create rotation for camera
Vector3D Camera::CreateRotation(float angle,Vector3D rotAxis)
{
	//set rotation angle
	float rotationAngle = angle * DirectX::XM_PI / 180;
	//Create axis
	DirectX::XMVECTOR axis = DirectX::XMVectorSet(rotAxis.show_X(), rotAxis.show_Y(), rotAxis.show_Z(), 0.0f);

	//Convert position to orgin vector
	DirectX::XMVECTOR origin = DirectX::XMVectorSet(_eye.show_X(), _eye.show_Y(), _eye.show_Z(), 0.0f);

	//Create rotation quaternion
	DirectX::XMVECTOR quaternionVector = DirectX::XMQuaternionRotationAxis(axis, rotationAngle);
	//calculate new postion
	DirectX::XMVECTOR XM_NewPos = DirectX::XMVector3Rotate(origin, quaternionVector);

	//store position
	DirectX::XMFLOAT4 temp;
	DirectX::XMStoreFloat4(&temp, XM_NewPos);

	//convert to vector3d and return
	return Vector3D(temp.x, temp.y, temp.z);

}


//delete references
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
	//convert current matrix to XMMatrix
	DirectX::XMFLOAT4X4 matrixConversion;
	for (int i = 0; i < _view.get_rows(); i++) {
		for (int j = 0; j < _view.get_cols(); j++) {
			matrixConversion.m[i][j] = _view(i,j);//pass in new projection Matrix values
		}
	}
	//return XMMatrix
	return matrixConversion;

}

DirectX::XMFLOAT4X4 Camera::GetProjectionXMMATRIX()
{
	//convert projection matrix to XMMatrix
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

void Camera::SetCameraMode(CameraMode NewMode)
{
	currMode = NewMode;
	//Evaluate new cam mode
	EvaluateCameraMode();
}

//move camerea towards at point
void Camera::Zoom(bool zoomIn) {
	if (_hasControl) {

		if (!_isLookingForward) {
			if (zoomIn) {
				//move camera in direction of target
				if (_eye.distance(_at) > _maxZoomDistance) {
					Vector3D moveDir = _at - _eye;

					SetPosition(_eye +(moveDir.normalization()*time)* cameraSpeed);
				}

			}
			else {
				//move camera in opposite direction of target
				Vector3D moveDir = _at - _eye;

				SetPosition(_eye - (moveDir.normalization()*time* cameraSpeed));
			}
		}
	}
}

//rotate on cameras Y axis
void Camera::RotateYAxis(bool rotateRight)
{

	if (_hasControl) {

		if (!_isLookingForward) {
			//flip rotation depending on what directions
			float rotDir =1.0f;
			if (rotateRight) {
				
				rotDir *= -1.0f;
			
			}
			//update direction
			SetPosition(CreateRotation(rotDir *time* cameraSpeed, Vector3D(0.0f, 1.0f, 0.0f)));
		}
	}
}

//rotate on cameras x axis
void Camera::RotateXAxis(bool rotateUp)
{
	if (_hasControl) {

		if (!_isLookingForward) {
			//flip rotation depending on what directions
			float rotDir = 1.0f;
			if (!rotateUp) {

				rotDir *= -1.0f;

			}

			SetPosition(CreateRotation(rotDir *time*cameraSpeed, Vector3D(1.0f, 0.0f, 0.0f)));

			
		}
	}
}
//rotate on cameras Y axis
void Camera::adjustPitch(bool isLookingUp)
{
	if (_hasControl) {
		if (_isLookingForward) {
			float newPitch = 0;
			//flip rotation depending on what directions
			if (isLookingUp) {
				newPitch = _rotation.show_X() - (time * cameraSpeed);
			}
			else {
				newPitch = _rotation.show_X() + (time * cameraSpeed);
			}
			_rotation = Vector3D(newPitch, _rotation.show_Y(), _rotation.show_Z());
		}
	}
}
//rotate on cameras Y axis
void Camera::adjustYaw(bool isLookingRight)
{
	if (_hasControl) {
		if (_isLookingForward) {
			//flip rotation depending on what directions
			float newYaw = 0;
			if (isLookingRight) {
				newYaw = _rotation.show_Y() + time * cameraSpeed;
			}
			else {
				newYaw = _rotation.show_Y() - time * cameraSpeed;
			}
			_rotation = Vector3D(_rotation.show_X(), newYaw, _rotation.show_Z());
		}
	}
}

//move camera along its local x axis
void Camera::Strafe(bool strafeRight)
{
	if (_hasControl) {
		if (_isLookingForward) {
			//flip direction depending on direction
			if (strafeRight) {
				
				Vector3D moveDir =  _eye+ (GetRight()  * cameraSpeed *time);
				SetPosition(moveDir);
			}
			else {
				Vector3D moveDir = _eye - (GetRight() * cameraSpeed *time);
				SetPosition(moveDir);
			}
		}
	}
}


//move camera along its local Z axis
void Camera::Move(bool moveForward)
{
	if (_hasControl) {
		if (_isLookingForward) {
			//flip direction depending on direction
			if (moveForward) {
				Vector3D moveDir = _eye + (_forward * cameraSpeed * time);
	
				SetPosition(moveDir);
			}
			else {
				Vector3D moveDir = _eye - (_forward * cameraSpeed * time);
				SetPosition(moveDir);
			}
		}
	}
}

//move camera along its global y axis
void Camera::MoveOnY(bool up)
{
	if (_hasControl) {

		//flip direction depending on direction
		if (up) {
			Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y() + (cameraSpeed*time ), _eye.show_Z());
			SetPosition(moveDir);
		}
		else {
			Vector3D moveDir = Vector3D(_eye.show_X(), _eye.show_Y() - (cameraSpeed* time ), _eye.show_Z());
			SetPosition(moveDir);
		}

	}
}

//set target look at
void Camera::SetLookAtGameObject(GameObject* target)
{
	targetGameObject = target;
}

//toggle between following
void Camera::ToggleFollow(bool isFollowing)
{
	_isFollowCam = isFollowing;
}

//begin following a path
void Camera::BeginFollowingPath()
{
	if (_hasControl&& !pathPoints.empty()) {
		_hasControl = false; //player can't move camera
		followPath = true;//is following

	}
}

//Set how fast camera moves along path
void Camera::SetPathSpeed(float newPathSpeed)
{

	pathSpeed = newPathSpeed;
}

//update path position
void Camera::FollowPath(float t)
{
	//direction to to path point
	Vector3D dir = pathPoints[currPathIndex] - _eye;
	Vector3D dirNORM = dir.normalization();

	//move to position relative to time
	SetPosition(_eye + dirNORM* pathSpeed * time);
	
	//If camera is approoximately close to point update path point
	if (_eye.distance(pathPoints[currPathIndex]) < 0.05 && _eye.distance(pathPoints[currPathIndex]) > -0.05){
		_eye = pathPoints[currPathIndex];
		currPathIndex++;
		
		//if at end of path
		if (currPathIndex >= pathPoints.size()) {
			//stop and reset path
			currPathIndex = 0;
			StopFollowingPath();
		}
				
	}
	
}

//stop following path
void Camera::StopFollowingPath()
{
	//regain control and stop following path
	_hasControl = true;
	followPath = false;
	//reset path speed
	pathSpeed = minPathSpeed;
}
//add point to vector
void Camera::AddPathPoint(Vector3D newPoint)
{
	pathPoints.push_back(newPoint);
}

//Evaluate what settings camera has depending on camera mode
void Camera::EvaluateCameraMode()
{
	//Apply changes depending on each mode
	switch (currMode)
	{
	case Camera::FirstPerson:
		//looks forward
		_isLookingForward = true;
		break;
	case Camera::ThirdPerson:

		//not look forward looks to target
		_isLookingForward = false;
		if (targetGameObject != nullptr) {
			SetPosition(targetGameObject->GetPosition() + followCamOffset);
			_rotation = targetGameObject->GetRotation();
		
		}
		//if it has no target set back to perspective
		else {
			SetCameraMode(Camera::Perspective);
		}
		break;
	case Camera::Perspective:
		_isLookingForward = false;
		//Looks at target position
		break;

	}
}




//====================================

void Camera::DisplayCameraGUI()
{
	
	//show settings of camera
	if (ImGui::Begin(cameraName.c_str())) {
		//Update display information based on camera mode
		switch (currMode) {
			case CameraMode::FirstPerson:
				ImGui::Text("Camera Mode: First Person Camera Mode");
				//display forward vector
				ImGui::Text(" Forward: x: %.1f y:%.1f z:%.1f", _forward.show_X(), _forward.show_Y(),
					_forward.show_Z());
			break;
			case CameraMode::ThirdPerson:
				ImGui::Text("Camera Mode: Third Person Mode");
				//highlight what camera looks at
				ImGui::Text("Target Position: x: %.1f y:%.1f z:%.1f", _at.show_X(), _at.show_Y(),
					_at.show_Z());
				break;
			case CameraMode::Perspective:
				ImGui::Text("Camera Mode: Perspective");
				//highlight what camera looks at
				ImGui::Text("Target Position: x: %.1f y:%.1f z:%.1f", _at.show_X(), _at.show_Y(),
					_at.show_Z());
				break;
		}

		//slider to allow user to slow down or speed up camera
		ImGui::SliderFloat("Camera Speed:", &cameraSpeed, 0.001f, 0.05f);

		//Allow users to set position of object from gui 
		float pos[3] = { _eye.show_X(),_eye.show_Y(),_eye.show_Z() };
		ImGui::InputFloat3("Position ", pos, "%.2f");
		SetPosition(Vector3D(pos[0], pos[1], pos[2]));


		//Allow users to set rotation of object from gui 
		float rot[3] = { _rotation.show_X(),_rotation.show_Y(),_rotation.show_Z() };
		ImGui::InputFloat3("Rotation ", rot, "%.2f");
		_rotation =(Vector3D(rot[0], rot[1], rot[2]));

		if (targetGameObject != nullptr) {
			ImGui::Text("Target Object: %s ", targetGameObject->GetGameObjectID().c_str());
		}
		else {
			ImGui::Text("Target Object: NONE");
		}
	}

	ImGui::End();

}

void Camera::EndGUI()
{
	ImGui::End();
}
