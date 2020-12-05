#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>
#include <iostream>
#include "DDSTextureLoader.h"
#include "InputManager.h"
#include "Structs.h"
#include "OBJLoader.h"
#include "Camera.h"
#include "GameObject.h"
#include "Commons.h"

//using namespace DirectX;



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
 	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;
	ID3D11Buffer*			_pPlaneVertexBuffer;
	ID3D11Buffer*			_pPlaneIndexBuffer;
	ID3D11Buffer*			_pPyramidVertexBuffer;
	ID3D11Buffer*			_pPyramidIndexBuffer;
	ID3D11Buffer*           _pConstantBuffer;
	XMFLOAT4X4              _world,_worldPlane;
	std::vector<XMFLOAT4X4> _worldMatrices;
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




private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	UINT _WindowHeight;
	UINT _WindowWidth;

	Vertex* tempVertexArr;


private:
	int minNumOfAstroids =100;
	int maxNumOfAstroids = 250;
	float objectMoveSpeed =1.0f;
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
	void ChangeCameraMode();

	void MoveObjectForward();
public:

	InputManager* _input;
	
private:
	Camera* GetActiveCamera();
	GameObject* GetSelectedObject();
	void SetNumberOfAStroid();
	void ScaleAndOffsetAstroids(float time);
	void DrawAstroids(ConstantBuffer cb);
	void CreateD11Vertex(Vertex3D arr[], int arrLength);
	static LRESULT CALLBACK MsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void InitCamera(Vector3D initPos, Vector3D lootAt, Vector3D up);
	void InitGameObject(Vector3D initPos,char* modelPath, ID3D11ShaderResourceView* texture);
	void DrawGameObjects(ID3D11DeviceContext* deviceContext, ConstantBuffer cb);
	void CleanUpCameras();
	void UpdateGameObjects(float time);
	void CleanUpGameObjects();
	void MoveActiveGameObject(Vector3D direction);

};

