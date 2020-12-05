#pragma once
#include "GameObject.h"
#include "Commons.h"
class GameObjectPrimitives : public GameObject {

public:
	enum class PrimitiveType
	{
		Cube,
		Plane,
		Pyramid
	};

public:
	GameObjectPrimitives(PrimitiveType geometricType, Vector3D initialPosition, ID3D11Device* device, ID3D11ShaderResourceView* texture);
	~GameObjectPrimitives() =default;

private:
	PrimitiveType gameObjectType;
	void CreateMeshData(Vertex3D vertexArr[], UINT vbCount, UINT IndexCount, ID3D11Device* device, WORD* meshIndexBuffer);
	
private:
	void InitMesh(ID3D11Device* device);
};
