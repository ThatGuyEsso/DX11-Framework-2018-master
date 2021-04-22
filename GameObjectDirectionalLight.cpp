#include "GameObjectDirectionalLight.h"

GameObjectDirectionalLight::GameObjectDirectionalLight(Vector3D initPosition,
	BaseLightData baseLightData, Vector3D lightDir) :GameObjectLight(initPosition, baseLightData)
{
	lightDirection = lightDir;
}

void GameObjectDirectionalLight::DrawGUI()
{
	if (ImGui::Begin("Directional Light Settings")) {

		//Set light colour and intensity
		float light[3] = { lightData.Diffuse.x,lightData.Diffuse.y,lightData.Diffuse.z};
		ImGui::InputFloat3("Light Colour & Intensity ", light, "%.2f");
		XMVECTOR intensity = XMVectorSet(light[0], light[1], light[2],lightData.Diffuse.w);
		XMFLOAT4 newLightIntensity;
		XMStoreFloat4(&newLightIntensity, intensity);
		SetDiffuse(newLightIntensity);

		//Set light direction to change angle
		float lightDir[3] = { lightDirection.show_X(), lightDirection.show_Y(), lightDirection.show_Z() };
		ImGui::InputFloat3("Light Direction", lightDir, "%.2f");
		SetLightDirection(Vector3D(lightDir[0], lightDir[1], lightDir[2]));

		//Specular light colour 
		float spec[3] = { lightData.Specular.x,lightData.Specular.y,lightData.Specular.z };
		ImGui::InputFloat3("Specular Colour", spec, "%.2f");
		XMVECTOR specularColour = XMVectorSet(spec[0], spec[1], spec[2], lightData.Diffuse.w);
		XMFLOAT4 newSpecular;
		XMStoreFloat4(&newSpecular, specularColour);
		SetSpecular(newSpecular);

		//Specular power slider
		ImGui::SliderFloat("Specular Power", &lightData.specularPower, 0.0f, 100.0f, "%.1f");
	}
	ImGui::End();
}
