#pragma once
#include "GameObjectLight.h"

class GameObjectPointLight :public GameObjectLight {

private: 
	PointLight pointLightData;

public:
	GameObjectPointLight(Vector3D initPosition,BaseLightData baseLightData, PointLight initPointLightData);
	~GameObjectPointLight() = default;
	
	//setters and getters
	PointLight GetPointLight() { return pointLightData; }
	void SetRange(float newRange) { pointLightData.Range = newRange; }
	void SetAttenuation(DirectX::XMFLOAT3 newAtt) { pointLightData.Att = newAtt; }
};