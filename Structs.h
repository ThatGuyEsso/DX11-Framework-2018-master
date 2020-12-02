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
	XMFLOAT2 UV;
	bool operator<(const Vertex other) const{
	return memcmp((void*)this, (void*)&other, sizeof(Vertex)) > 0;
	};
};

struct Material {
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Colour;
};

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