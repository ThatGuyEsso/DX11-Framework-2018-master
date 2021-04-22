#include "GameObjectLight.h"

GameObjectLight::GameObjectLight(Vector3D initPosition, BaseLightData lightData) :GameObject("",initPosition,nullptr,nullptr)
{
	this->lightData = lightData;
}

GameObjectLight::~GameObjectLight()
{
}

void GameObjectLight::DrawGUI()
{
}
