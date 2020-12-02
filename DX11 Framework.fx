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

//cbuffer cbPerFrane {
//	DirectionalLight gDirLight;
//	PointLight gPointLight;
//	SpotLight gSpotLight;
//	float gEyePosW;
//};


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

	output.Pos = mul(Pos, mWorld);
	output.PosW = output.Pos;
	////Compute specular
	float3 toEye = normalize(EyePosW - output.Pos.xyz); // Compute the vector from the vertex to the eye position.

	output.Pos = mul(output.Pos, mView);
	output.Pos = mul(output.Pos, mProjection);
	
	output.TexC = Tex;
	float3 normalW = mul(float4(NormalL, 0.0f), mWorld).xyz;
	normalW = normalize(normalW);
	output.Norm = normalW;

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

	////initialise lighting terms
	//ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	//ComputeDirectionalLight(DiffuseMtrl,)
	float3 r = reflect(-LightVecW, input.Norm);


	// Determine how much (if any) specular light makes it
	// into the eye.
	float specularAmount = pow(max(dot(r, toEye), 0.0f), SpecularPower);
	float3 specular = specularAmount * (SpecularMtrl * SpecularLight).rgb;
	// Compute Colour using Diffuse lighting only
	float diffuseAmount = max(dot(LightVecW, input.Norm), 0.0f);
	//Compute ambient
	float ambient = (AmbientMtrl * ambientCol).rgb;
	
	float4 textureColour = txDiffuse.Sample(samLinear, input.TexC);
	float4 col;
	col = float4(textureColour+/*specular*/ /*+*/ /*ambient*/ + diffuseAmount * (DiffuseMtrl * DiffuseLight).rgb, DiffuseMtrl.a);
    return col;
}
