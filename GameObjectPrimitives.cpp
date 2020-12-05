#include "GameObjectPrimitives.h"

GameObjectPrimitives::GameObjectPrimitives(PrimitiveType geometricType, Vector3D initialPosition,
ID3D11Device* device, ID3D11ShaderResourceView* texture):GameObject(nullptr,initialPosition,device,texture)
{
	gameObjectType = geometricType;
}

void GameObjectPrimitives::InitMesh()
{
	switch (gameObjectType)
	{
	case GameObjectPrimitives::Cube:

		break;
	case GameObjectPrimitives::Plane:
		break;
	case GameObjectPrimitives::Pyramid:
		break;
	default:
		break;
	}
}
