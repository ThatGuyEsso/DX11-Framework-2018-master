#pragma once
#include "OBJLoader.h"
#include "Commons.h"

#include <string>
#include "Vector3D.h"
#include <d3d11_1.h>
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include "vendor/imgui/imgui_impl_dx11.h"

class GameObject {
protected:
	MeshData _ObjectModel;
	Material _Material = DefaultMaterial;
	Vector3D _Position;
	Vector3D _Scale;
	Vector3D _Rotation;
	
	
	float _orbitSpeed;
	float _orbitalSpeed;
	float _daySpeed;
	float moveSpeed =0.01f;
	bool isOrbiting = false;

	std::string _gameObjectID;

	ID3D11ShaderResourceView* _pTextureRV = nullptr;

	XMFLOAT4X4 _OWorld;

	float gTime;
public:
	GameObject(char* filePath,Vector3D initialPosition, ID3D11Device* device, ID3D11ShaderResourceView* texture);
	~GameObject();
	virtual void Update(float time);
	void Draw(ID3D11DeviceContext* _pImmediateContext);
	void DrawGUI();
	void Move(Vector3D dir);
	Vector3D GetPosition();
	MeshData GetMesh();
	Material GetMaterial() { return _Material; }
	ID3D11ShaderResourceView* GetTexture(); 
	XMFLOAT4X4 GetObjectWorldMatrix();
	std::string GetGameObjectID() { return _gameObjectID; }
	Vector3D GetRotation() { return _Rotation; };
	void BeginOrbit(float daySpeed, float orbitalSpeed);

	void SetOrbitSpeed(float newSpeed);
	void SetPosition(Vector3D newPos);

	void SetRotation(Vector3D newRot);
	void SetGameObjectID(std::string newID, int UID);
	void SetLocalScale(Vector3D newScale);
	void SetMaterial(Material newMat) { _Material = newMat; }

};