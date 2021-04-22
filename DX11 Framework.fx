//--------------------------------------------------------------------------------------
// File: DX11 Framework.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
//#include "LightHelper.h"

//--------------------------------------------------------------------------------------
// Texture sampling variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
	matrix mWorld;
	matrix mView;
	matrix mProjection;
	float4 DiffuseMtrl;
	float4 DiffuseLight;
	float3 LightVecW;
	float SpecularPower;
	float4 SpecularMtrl;
	float4 SpecularLight;
	float4 AmbientMtrl;
	float4 ambientCol;
	float3 EyePosW;
	float gTime;


};

//not currently in use
struct PointLight {

	float3 dir;
	float3 pos;
	float  range;
	float3 att;
	float4 ambient;
	float4 diffuse;
	
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float3 PosW : POSITION;
	float2 TexC : TEXCOORD0;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float4 Pos : POSITION, float3 NormalL : NORMAL, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//vertex position
	output.Pos = mul(Pos, mWorld);
	//return output position
	output.PosW = output.Pos;

	float3 toEye = normalize(EyePosW - output.Pos.xyz); // Compute the vector from the vertex to the eye position.

	output.Pos = mul(output.Pos, mView);
	output.Pos = mul(output.Pos, mProjection);
	
	output.TexC = Tex;//return tex coordinate
	//return vertex normal
	float3 normalW = mul(float4(NormalL, 0.0f), mWorld).xyz;
	normalW = normalize(normalW);
	output.Norm = normalW;

	//return vertex data
	return output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
	//input.Norm = normalize(input.Norm);

	float3 toEye = normalize(EyePosW - input.Pos.xyz); // Compute the vector from the vertex to the eye position.
	// Compute the reflection vector.

	float3 r = reflect(-LightVecW, input.Norm);

	// Determine how much (if any) specular light makes it
	// into the eye.
	float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
	float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;
	// Compute Colour using Diffuse lighting only
	float diffuseAmount = max(dot(LightVecW, input.Norm), 0.0f)*(DiffuseMtrl * DiffuseLight);
	//Compute ambient
	float ambient = (AmbientMtrl * ambientCol).rgb;
	
	//add all lighting models
	float appliedLight = ambient + diffuseAmount + specular;
	//create texutre
	float3 textureColour = txDiffuse.Sample(samLinear, input.TexC);
	float4 col;//colour of pixel
	//multiply pixel colour with texture colour
	col = float4(textureColour* appliedLight, DiffuseMtrl.a);
    return col;
}
