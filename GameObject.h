#pragma once
#include "OBJLoader.h"
#include <string>
#include "Vector3D.h"
#include <d3d11_1.h>


class GameObject {
private:
	MeshData _ObjectModel;
	Vector3D _Position;
	Vector3D _Scale;
	Vector3D _Rotation;
	Material _material;
	ID3D11ShaderResourceView* _pTextureRV = nullptr;

	XMFLOAT4X4 _OWorld;
public:
	GameObject(char* filePath,Vector3D initialPosition, ID3D11Device* device, ID3D11ShaderResourceView* texture);
	~GameObject() = default;
	void Update(float time);
	void Draw(ID3D11DeviceContext* _pImmediateContext);

	Vector3D GetPosition();
	MeshData GetMesh();
	ID3D11ShaderResourceView* GetTexture(); 
	XMFLOAT4X4 GetObjectWorldMatrix();
	void SetPosition(Vector3D newPos);
private:
};