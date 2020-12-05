#pragma once
#include "GameObject.h"

class GameObjectPrimitives : public GameObject {

public:
	enum PrimitiveType
	{
		Cube,
		Plane,
		Pyramid
	};

public:
	GameObjectPrimitives(PrimitiveType geometricType, Vector3D initialPosition, ID3D11Device* device, ID3D11ShaderResourceView* texture);
	~GameObjectPrimitives();

private:
	PrimitiveType gameObjectType;
private:
	void InitMesh();
};
