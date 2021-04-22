#pragma once
#include "GameObjectLight.h"


class GameObjectDirectionalLight : public GameObjectLight {

public:
	GameObjectDirectionalLight(Vector3D initPosition,BaseLightData baseLightData, Vector3D lightDir);
	~GameObjectDirectionalLight() = default;

	Vector3D GetLightDirection() { return lightDirection; };
	void DrawGUI();
	void SetLightDirection(Vector3D newDirection) {  lightDirection = newDirection; }

private:
	Vector3D lightDirection;

};