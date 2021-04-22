#pragma once

#include "Vector3D.h"


using namespace DirectX;

struct Vertex3D
{
	Vector3D Pos;
	XMFLOAT2 UV;

};
struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;
	bool operator<(const Vertex other) const{
	return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
	};
};

//struct SimpleVertex
//{
//	XMFLOAT3 Pos;
//	XMFLOAT4 Colour;
//};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 DiffuseLight;
	XMFLOAT3 LightVecW;
	float SpecularPower;
	XMFLOAT4 SpecularMtrl;
	XMFLOAT4 SpecularLight;
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 AmbientCol;
	XMFLOAT3 EyePosW;
	float gTime;
};

struct BaseLightData {
	BaseLightData() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	float specularPower;

	//Struct constructor
	BaseLightData(DirectX::XMFLOAT4 initAmbient,DirectX::XMFLOAT4 initDiffuse, DirectX::XMFLOAT4 initSpecular,float initSpecularPower) {
		Ambient = initAmbient;
		Diffuse = initDiffuse;
		Specular = initSpecular;
		specularPower = initSpecularPower;
	}
};

struct PointLight {
	PointLight() { ZeroMemory(this, sizeof(this)); }
	DirectX::XMFLOAT3 Att;
	float Range;
	PointLight(XMFLOAT3 attenuation, float initRange) {
		Att = attenuation;
		Range= initRange;
	}


};

struct Material {
	DirectX::XMFLOAT4 diffuseMaterial;
	DirectX::XMFLOAT4 ambientMaterial;
	DirectX::XMFLOAT4 specularMat;
};