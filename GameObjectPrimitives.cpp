#include "GameObjectPrimitives.h"

GameObjectPrimitives::GameObjectPrimitives(PrimitiveType geometricType, Vector3D initialPosition,
ID3D11Device* device, ID3D11ShaderResourceView* texture):GameObject("",initialPosition,device,texture)
{
	gameObjectType = geometricType;
	InitMesh(device);
}


void GameObjectPrimitives::InitMesh(ID3D11Device* device)
{
	switch (gameObjectType)
	{
	case PrimitiveType::Cube:
		CreateMeshData(CubeVertices3D, CubeVertexCount, CubeIndexCount, device, CubeIndices);
		break;
	case PrimitiveType::Plane:
		break;
	case PrimitiveType::Pyramid:
		break;
	default:
		break;
	}
}


void GameObjectPrimitives::CreateMeshData(Vertex3D vertexArr[], UINT vbCount, UINT IndexCount, ID3D11Device* device, WORD* meshIndexBuffer)
{

	Vertex* tempVertexArr = new Vertex[vbCount];

	int triangleVertexCount = 0;
	//int vertexIndex = 0;//start at index one  (second element)
	Vector3D vec1;
	Vector3D vec2;
	//Add all positions
	for (int i = 0; i < vbCount; i++) {
		//Pass to  position and UV to array
		tempVertexArr[i].Pos = XMFLOAT3(vertexArr[i].Pos.show_X(), vertexArr[i].Pos.show_Y(), vertexArr[i].Pos.show_Z());
		tempVertexArr[i].TexC = vertexArr[i].UV;
		//if it is the first vertex of the triangle 
		if (triangleVertexCount == 0) {
			vec1 = vertexArr[i + 1].Pos - vertexArr[i].Pos;//the index positions are both greater
			vec2 = vertexArr[i + 2].Pos - vertexArr[i].Pos;
			Vector3D cross = vec1.cross_product(vec2);
			Vector3D crossNormalised = cross.normalization();
			//Upload normal data
			tempVertexArr[i].Normal = XMFLOAT3(crossNormalised.show_X(), crossNormalised.show_Y(), crossNormalised.show_Z());
			triangleVertexCount++;
			//if it is the second vertex
		}
		else if (triangleVertexCount == 1) {
			vec1 = vertexArr[i - 1].Pos - vertexArr[i].Pos;// go back to origin
			vec2 = vertexArr[i + 2].Pos - vertexArr[i].Pos;
			Vector3D cross = vec1.cross_product(vec2);
			Vector3D crossNormalised = cross.normalization();
			//Upload normal data
			tempVertexArr[i].Normal = XMFLOAT3(crossNormalised.show_X(), crossNormalised.show_Y(), crossNormalised.show_Z());
			triangleVertexCount++;
		}
		//3rd vertex
		else if (triangleVertexCount == 2) {
			vec1 = vertexArr[i - 1].Pos - vertexArr[i].Pos;//go to orign of triangle
			vec2 = vertexArr[i - 2].Pos - vertexArr[i].Pos;// go to previous vertex
			Vector3D cross = vec1.cross_product(vec2);
			Vector3D crossNormalised = cross.normalization();
			//Upload normal data
			tempVertexArr[i].Normal = XMFLOAT3(crossNormalised.show_X(), crossNormalised.show_Y(), crossNormalised.show_Z());

			float sumOfX = tempVertexArr[i].Normal.x + tempVertexArr[i - 1].Normal.x + tempVertexArr[i - 2].Normal.x;
			float sumOfY = tempVertexArr[i].Normal.y + tempVertexArr[i - 1].Normal.y + tempVertexArr[i - 2].Normal.y;
			float sumOfZ = tempVertexArr[i].Normal.z + tempVertexArr[i - 1].Normal.z + tempVertexArr[i - 2].Normal.z;

			float  averageX = sumOfX / 3.0f;
			float  averageY = sumOfY / 3.0f;
			float  averageZ = sumOfZ / 3.0f;

			tempVertexArr[i - 2].Normal = XMFLOAT3(averageX, averageY, averageZ);
			triangleVertexCount == 0;
		}
	}

	//Create vertexbuffer
	ID3D11Buffer* vertexBuffer;
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * vbCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = tempVertexArr;

	hr = device->CreateBuffer(&bd, &InitData, &vertexBuffer);

	//Upload data to mesh
	_ObjectModel.VertexBuffer = vertexBuffer;
	_ObjectModel.VBOffset = 0;
	_ObjectModel.VBStride = sizeof(Vertex);

	//creatte vertex data
	ID3D11Buffer* indexBuffer;

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * IndexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshIndexBuffer;
	device->CreateBuffer(&bd, &InitData, &indexBuffer);
	//Upload data to mesh
	_ObjectModel.IndexCount = IndexCount;
	_ObjectModel.IndexBuffer = indexBuffer;

	delete tempVertexArr;
	tempVertexArr = nullptr;
}