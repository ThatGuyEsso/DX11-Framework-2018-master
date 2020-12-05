#pragma once
#include <windows.h>
#include "Structs.h"
#include <directxmath.h>
#ifndef _COMMONS_H
#define _COMMONS_H

//Defaults
static const wchar_t* defaultTexturePath = L"Assets/Textures/Crate_COLOR.dds";

static UINT CubeIndexCount = 36;
static UINT CubeVertexCount =8;
static UINT PlaneIndexCount = 96;
static UINT PlaneVertexCount = 25;
static UINT PyramidIndexCount= 24;
static UINT PyramidVertexCount= 5;
//// Create index buffer
static WORD CubeIndices[] =
{
	0,2,3,
	3,1,0,

	5,4,1,
	1,3,5,

	2,0,7,
	7,6,2,

	3,2,6,
	6,5,3,

	4,5,6,
	6,7,4,

	7,0,1,
	1,4,7


};

static Vertex3D CubeVertices3D[] =
{
	{Vector3D(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT2(0.0f,0.0f)},
	{Vector3D(0.5f, 0.5f, 0.5f),DirectX::XMFLOAT2(1.0f,0.0f)},
	{Vector3D(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT2(0.0f,1.0f)},
	{Vector3D(0.5f,-0.5f, 0.5f), DirectX::XMFLOAT2(1.0f,1.0f)},
	{Vector3D(0.5f,0.5f, -0.5f), DirectX::XMFLOAT2(1.0f,0.0f)},
	{Vector3D(0.5f,-0.5f, -0.5f),DirectX::XMFLOAT2(1.0f,1.0f)},
	{Vector3D(-0.5f,-0.5f, -0.5f),DirectX::XMFLOAT2(0.0f,1.0f)},
	{Vector3D(-0.5f,0.5f, -0.5f),DirectX::XMFLOAT2(0.0f,0.0f)}
};

static WORD PlaneIndicies[] =
{
	// row	1
			//one square
			0,1,6, //0
			6,5,0, //1
			//two square
			1,2,7, //2
			7,6,1, //3
			// 3rd square
			2,3,8,//4
			8,7,2,//5
			//4th square
			3,4,9,//6
			9,8,3,//7


			// row 2
			5,6,11,//8
			11,10,5,//9

			6,7,12,//10
			12,11,6,//11

			7,8,13,//12
			13,12,7,//13

			8,9,14,//14
			14,13,8,//15

			// row 3
			10,11,16,//16
			16,15,10,//17

			11,12,17,//18
			17,16,11,//19

			12,13,18,//20
			18,17,12,//21

			13,14,19,//22
			19,18,13,//23

			// row 4
			15,16,21,//24
			21,20,15,//25


			16,17,22,//26
			22,21,16,//27

			17,18,23,//28
			23,22,17,//29

			18,19,24,//30
			24,23,18//31

};
#endif