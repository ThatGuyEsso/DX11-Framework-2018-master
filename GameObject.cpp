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
	//Update world
	XMStoreFloat4x4(&_OWorld, XMMatrixScaling(_Scale.show_X(), _Scale.show_Y(), _Scale.show_Z())
		* XMMatrixRotationX(_Rotation.show_X()) * XMMatrixRotationY(_Rotation.show_Y()) * XMMatrixRotationZ(_Rotation.show_Z())
		* XMMatrixTranslation(_Position.show_X(), _Position.show_Y(), _Position.show_Z()) * XMMatrixRotationY(_orbitSpeed));
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

void GameObject::Move(Vector3D dir, float moveSpeed)
{
	Vector3D newPosition = _Position + (dir *moveSpeed* gTime);
	_Position = newPosition;
}
