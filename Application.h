#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>
#include<string>
#include <iostream>
#include "DDSTextureLoader.h"
#include "InputManager.h"
#include "Structs.h"
#include "OBJLoader.h"
#include "Camera.h"
#include "Commons.h"
#include "GameObjectPrimitives.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "GUIManager.h"

using namespace rapidxml;


class Application
{
private:
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;
	XMFLOAT4X4              _world;
	ID3D11Buffer*           _pConstantBuffer;

	std::vector<GameObjectPrimitives*> _astroids;
	std::vector<float>		_astroidScales;
	std::vector<float>		_astroidOffset;
	std::vector<float>		_astroidRotationSpeedScalar;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D*		_depthStencilBuffer;
	XMFLOAT4X4              _view;
	XMFLOAT4X4              _projection;
	ID3D11RasterizerState* _wireFrame;
	ID3D11RasterizerState* _solidMode;
	float gTime;
	XMFLOAT3 lightDirection;
	XMFLOAT4 diffuseMaterial;
	XMFLOAT4 diffuseLight;
	XMFLOAT4 ambientMaterial;
	XMFLOAT4 ambientColor;
	XMFLOAT4 specularMat;
	XMFLOAT4 specularLight;
	XMFLOAT3 eyePosW;
	float specularPower;


	ID3D11ShaderResourceView* _pTextureRV = nullptr;
	ID3D11ShaderResourceView* _pDefaultTextureRV = nullptr;
	ID3D11SamplerState* _pSamplerLinear = nullptr;
	GUIManager* _guiManager;



private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	void InitGame();

	void Cleanup();

	UINT _WindowHeight;
	UINT _WindowWidth;


private:
	int minNumOfAstroids =100;
	int maxNumOfAstroids = 250;
	float objectMoveSpeed =0.5f;
	MeshData testData;
	float _NearDepth = 0.01f, _FarDepth = 100.0f;
	bool isWireFrameModeOn = false;
	int activeCamIndex =0;
	int selectedGameObjectIndex = 0;
	std::vector<Camera*> cameras;
	std::vector<GameObject*> _gameObjects;
public:
	void ToggleWireFrame();
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);
	void CycleBetweenCameras();
	void CycleBetweenGameObjects();
	void SetActiveCamera(int index);
	void Update();
	void Draw();
	void ZoomActiveCamera(bool zoomIn);
	void StrafeActiveCamera(bool strafeRight);
	void MoveActiveCamAlongY(bool up);
	void OrbitCameraY(bool orbitRight);
	void OrbitCameraX(bool orbitUP);
	void RotateCameraYaw(bool lookRight);
	void RotateCameraPitch(bool lookUp);
	void ActiveCameraStartPath();
	void ChangeCameraMode();
	void SetActiveCameraTargetGameObject();

	void MoveObjectForward();
public:

	InputManager* _input;
	
private:
	void CreateScene(std::string levelName);
	Camera* GetActiveCamera();
	GameObject* GetSelectedObject();
	void SetNumberOfAStroid();
	void ScaleAndOffsetAstroids(float time);
	void DrawAstroids(ConstantBuffer cb, ID3D11DeviceContext* deviceContext);
	void CreateD11Vertex(Vertex3D arr[], int arrLength);
	static LRESULT CALLBACK MsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Camera* InitCamera(Vector3D initPos, Vector3D lookAt, Vector3D up);
	GameObject* InitGameObject(Vector3D initPos,char* modelPath, ID3D11ShaderResourceView* texture);
	GameObjectPrimitives* InitPrimitiveGameObject(GameObjectPrimitives::PrimitiveType objectType,Vector3D initPos, ID3D11ShaderResourceView* texture);
	void DrawGameObjects(ID3D11DeviceContext* deviceContext, ConstantBuffer cb);
	void CleanUpCameras();
	void UpdateGameObjects(float time);
	void CleanUpGameObjects();
	void MoveActiveGameObject(Vector3D direction);
	xml_node<>* FindChildNode(xml_node<>* parent, const std::string& type, const std::string& attrib, const std::string value);
	void CompileLevelData(xml_node<>* data);
	void InitLevelData(xml_node<>* node, const std::string nodeType);
	Vector3D ConvertCharToVector3D(const std::string newX, const std::string newY, const std::string newZ);
	void InitGameObjectFromNode(xml_node<>* gameObjectNode);
	void InitPrimitiveGameObjectFromNode(xml_node<>* gameObjectNode);
	void InitCameraFromNode(xml_node<>* gameObjectNode);
};

