#pragma once
#include "OBJLoader.h"
#include <string>
#include "Vector3D.h"
#include <d3d11_1.h>


class GameObject {
protected:
	MeshData _ObjectModel;
	Vector3D _Position;
	Vector3D _Scale;
	Vector3D _Rotation;
	float _orbitSpeed;
	Material _material;
	ID3D11ShaderResourceView* _pTextureRV = nullptr;

	XMFLOAT4X4 _OWorld;

	float gTime;
public:
	GameObject(char* filePath,Vector3D initialPosition, ID3D11Device* device, ID3D11ShaderResourceView* texture);
	~GameObject();
	void Update(float time);
	void Draw(ID3D11DeviceContext* _pImmediateContext);
	void Move(Vector3D dir, float moveSpeed);
	Vector3D GetPosition();
	MeshData GetMesh();
	ID3D11ShaderResourceView* GetTexture(); 
	XMFLOAT4X4 GetObjectWorldMatrix();
	void SetOrbitSpeed(float newSpeed);
	void SetPosition(Vector3D newPos);

	void SetRotation(Vector3D newRot);

	void SetLocalScale(Vector3D newScale);

};