#pragma
#include <windows.h>
#include <directxmath.h>
#include "Vector3D.h"
#include "GameObject.h"
#include "Structs.h"


class GameObjectLight : public GameObject
{
protected:
	BaseLightData lightData;
public:
	//constructor take a position to place light and base light data
	GameObjectLight(Vector3D initPosition, BaseLightData lightData);
	~GameObjectLight();

	//GUI
	virtual void DrawGUI();


	//Setters and Getters
	BaseLightData GetLightData() { return lightData; }
	void SetAmbient(XMFLOAT4 newAmbient) { lightData.Ambient = newAmbient; }

	void SetDiffuse(XMFLOAT4 newDiffuse) { lightData.Diffuse = newDiffuse; }

	void SetSpecular(XMFLOAT4 newSpecular) { lightData.Specular = newSpecular; }

	void SetSpecularPower(float newPower) { lightData.specularPower = newPower; }

};

