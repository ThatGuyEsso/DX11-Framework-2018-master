#include "GameObject.h"

GameObject::GameObject(char* filePath, Vector3D initialPosition, ID3D11Device* device, ID3D11ShaderResourceView* texture): _pTextureRV(texture)
{
	_ObjectModel = OBJLoader::Load(filePath, device,false);

	_Position = initialPosition;
	_Scale = Vector3D(1.0f, 1.0f, 1.0f);
	_Rotation = Vector3D();
	_orbitSpeed =0.0f;
	XMStoreFloat4x4(&_OWorld, XMMatrixIdentity());
}
GameObject::~GameObject()
{

	_pTextureRV = nullptr;
}
void GameObject::Update(float time)
{
	gTime = time;

	//orbit mode
	if (isOrbiting) {
		SetOrbitSpeed(gTime* _orbitalSpeed);
		XMStoreFloat4x4(&_OWorld, XMMatrixScaling(_Scale.show_X(), _Scale.show_Y(), _Scale.show_Z())
			* XMMatrixRotationX(_Rotation.show_X()) * XMMatrixRotationY(_daySpeed* gTime) * XMMatrixRotationZ(_Rotation.show_Z())
			* XMMatrixTranslation(_Position.show_X(), _Position.show_Y(), _Position.show_Z()) * XMMatrixRotationY(_orbitSpeed));
	}
	//normal mode
	else {
		XMStoreFloat4x4(&_OWorld, XMMatrixScaling(_Scale.show_X(), _Scale.show_Y(), _Scale.show_Z())
			* XMMatrixRotationX(_Rotation.show_X()) * XMMatrixRotationY(_Rotation.show_Y()) * XMMatrixRotationZ(_Rotation.show_Z())
			* XMMatrixTranslation(_Position.show_X(), _Position.show_Y(), _Position.show_Z()) * XMMatrixRotationY(_orbitSpeed));
	}
	//Update world


}



Vector3D GameObject::GetPosition()
{
	return _Position;
}

MeshData GameObject::GetMesh()
{
	return _ObjectModel;
}

ID3D11ShaderResourceView* GameObject::GetTexture()
{
	return _pTextureRV;
}

XMFLOAT4X4 GameObject::GetObjectWorldMatrix()
{
	return _OWorld;
}

void GameObject::BeginOrbit(float daySpeed, float orbitalSpeed)
{
	_daySpeed = daySpeed;
	_orbitalSpeed = orbitalSpeed;
	isOrbiting = true;
}

void GameObject::SetOrbitSpeed(float newSpeed)
{
	_orbitSpeed = newSpeed;
}

void GameObject::SetPosition(Vector3D newPos)
{
	_Position = newPos;
}
void GameObject::SetRotation(Vector3D newRot)
{
	_Rotation = newRot;
}
void GameObject::SetGameObjectID(std::string newID, int UID)
{
	std::string ID = newID.append(std::to_string(UID));
	_gameObjectID = newID;
}
void GameObject::SetLocalScale(Vector3D newScale)
{
	_Scale = newScale;
}

void GameObject::Draw(ID3D11DeviceContext* _pImmediateContext)
{


	//Get reference to buffer
	_pImmediateContext->IASetVertexBuffers(0, 1, &_ObjectModel.VertexBuffer, &_ObjectModel.VBStride, &_ObjectModel.VBOffset);
	// Set index buffer
	_pImmediateContext->IASetIndexBuffer(_ObjectModel.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	_pImmediateContext->DrawIndexed(_ObjectModel.IndexCount, 0, 0);
}

void GameObject::DrawGUI()
{
	//Display game object id
	if (ImGui::Begin(_gameObjectID.c_str())) {

		//slider to allow user to slow down or speed up gamw object
		ImGui::SliderFloat("GameObject Speed:", &moveSpeed, 0.001f, 0.05f);

		//Allow users to set position of object from gui 
		float pos[3] = {_Position.show_X(),_Position.show_Y(),_Position.show_Z() };
		ImGui::InputFloat3("Position ", pos, "%.2f");
		SetPosition(Vector3D(pos[0], pos[1], pos[2]));
	
		//Allow users to set rotation of object from gui 
		float rot[3] = {_Rotation.show_X(),_Rotation.show_Y(),_Rotation.show_Z()};
		ImGui::InputFloat3("Rotation ", rot, "%.2f");
		SetRotation(Vector3D(rot[0], rot[1], rot[2]));

		//Allow users to set scale of object from gui 
		float scale[3] = { _Scale.show_X(),_Scale.show_Y(),_Scale.show_Z() };
		ImGui::InputFloat3("Scale ", scale, "%.2f");
		SetLocalScale(Vector3D(scale[0], scale[1], scale[2]));

	}
	ImGui::End();

}

//move in given direction
void GameObject::Move(Vector3D dir)
{
	Vector3D newPosition = _Position + (dir * moveSpeed * gTime);
	_Position = newPosition;
}
