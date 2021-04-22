#include "GameObjectPointLight.h"

GameObjectPointLight::GameObjectPointLight(Vector3D initPosition, BaseLightData baseLightData, PointLight initPointLightData)
	:GameObjectLight(initPosition,baseLightData)
{
	pointLightData = initPointLightData;
}
