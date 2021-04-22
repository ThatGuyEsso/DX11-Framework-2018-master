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
#include "GameObjectDirectionalLight.h"
using namespace rapidxml;


class Application
{
private:
//windows and d311 buffers
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
	ID3D11Buffer*           _pConstantBuffer;

	XMFLOAT4X4              _world;
	//astriod data
	std::vector<float>		_astroidScales;
	std::vector<float>		_astroidOffset;
	std::vector<float>		_astroidRotationSpeedScalar;

	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D*		_depthStencilBuffer;
	//render modes
	ID3D11RasterizerState* _wireFrame;
	ID3D11RasterizerState* _solidMode;
	float gTime;


	UINT _WindowHeight;
	UINT _WindowWidth;
	//Texture pointers
	ID3D11ShaderResourceView* _pTextureRV = nullptr;
	ID3D11ShaderResourceView* _pDefaultTextureRV = nullptr;
	ID3D11SamplerState* _pSamplerLinear = nullptr;

	//manager pointers



private:
	//initiation functions
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();

	//game object/component functions
	Camera* InitCamera(Vector3D initPos, Vector3D lookAt, Vector3D up);
	GameObject* InitGameObject(Vector3D initPos,char* modelPath, ID3D11ShaderResourceView* texture);
	GameObjectPrimitives* InitPrimitiveGameObject(GameObjectPrimitives::PrimitiveType objectType,
		Vector3D initPos, ID3D11ShaderResourceView* texture);
	GameObjectDirectionalLight* InitDirectionalLight(Vector3D initPosition, BaseLightData lightData, Vector3D lightDirection);
	//Init game functions
	void InitGame();
	void CreateScene(std::string levelName);
	//clean up
	void Cleanup();
	void CleanUpCameras();
	void CleanUpGameObjects();


private:
	//astroid proc gen data
	int minNumOfAstroids =300;
	int maxNumOfAstroids = 500;

	//camera data and render mode
	bool isWireFrameModeOn = false;
	float _NearDepth = 0.01f, _FarDepth = 100.0f;

	//Game object management data
	int activeCamIndex =0;
	int selectedGameObjectIndex = 0;
	int selectedLightIndex = 0;

	//Game object buffers
	std::vector<Camera*> cameras;
	std::vector<GameObject*> _gameObjects;
	std::vector<GameObject*> _astroids;
	std::vector<GameObjectLight*> _lights;
	//Lights
	GameObjectDirectionalLight* directionalLight;
	//active scene data
	std::string activeSceneName;
public:
	Application();
	~Application();
	
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);
	//render controls
	void ToggleWireFrame();
	void CycleBetweenLights();
	//Update/Draw
	void Update();
	void Draw();
	void RenderGUIs();
	void AddObjectGUI();

	//camera controls
	void ZoomActiveCamera(bool zoomIn);
	void StrafeActiveCamera(bool strafeRight);
	void MoveActiveCamAlongY(bool up);
	void OrbitCameraY(bool orbitRight);
	void OrbitCameraX(bool orbitUP);
	void RotateCameraYaw(bool lookRight);
	void RotateCameraPitch(bool lookUp);
	void ActiveCameraStartPath();
	void ChangeCameraMode();
	void CycleBetweenCameras();
	
	//Object Control
	void MoveObjectForward();
	void MoveObjectBackwards();
	void MoveObjectRight();
	void MoveObjectLeft();
	void CycleBetweenGameObjects();

	//setters
	void SetActiveCamera(int index);
	void SetActiveCameraTargetGameObject();

public:

	InputManager* _input;
	GUIManager* _guiManager;
	
private:
	//object control functions
	void MoveActiveGameObject(Vector3D direction);

	//Getters
	Camera* GetActiveCamera();
	GameObject* GetSelectedObject();
	GameObjectLight* GetSelectedLight();
	//Setters
	void SetNumberOfAsteroids();

	//update functions
	void UpdateGameObjects(float time);
	void ScaleAndOffsetAsteroids(float time);

	//Draw Functions
	void DrawAsteroids(ConstantBuffer cb, ID3D11DeviceContext* deviceContext);
	void DrawGameObjects(ID3D11DeviceContext* deviceContext, ConstantBuffer cb);


	//utility functions
	Vector3D ConvertCharToVector3D(const std::string newX, const std::string newY, const std::string newZ);
	xml_node<>* FindChildNode(xml_node<>* parent,
		const std::string& type, const std::string& attrib, const std::string value);

	//XML data compilation

	void InitGameObjectFromNode(xml_node<>* gameObjectNode);
	void InitPrimitiveGameObjectFromNode(xml_node<>* gameObjectNode);
	void InitCameraFromNode(xml_node<>* gameObjectNode);
	void InitLevelData(xml_node<>* node, const std::string nodeType);
	void CompileLevelData(xml_node<>* data);
	std::string GetActiveScene();

	//windows procedures
	static LRESULT CALLBACK MsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//Shaders
	void ApplyWorldViewProj(ConstantBuffer* cbuffer, XMMATRIX world, XMMATRIX view, XMMATRIX proj);
	void ApplyDirectionalLight(ConstantBuffer* cbuffer, GameObjectDirectionalLight* light, Vector3D viewPortPos);
	
};

