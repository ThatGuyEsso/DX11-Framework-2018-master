#include "Application.h"
Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;

	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	_pVertexBuffer = nullptr;
	_pIndexBuffer = nullptr;
	_pConstantBuffer = nullptr;
	isWireFrameModeOn = false;
	_input->instance();
	InitCamera(Vector3D(0.0f, 0.0f, -5.0f), Vector3D(), Vector3D(0.0f, 1.0f, 0.0f));
	InitCamera(Vector3D(5.0f, 4.0f, -2.0f), Vector3D(), Vector3D(0.0f, 1.0f, 0.0f));
	InitCamera(Vector3D(5.0f, -10.0f, 5.0f), Vector3D(), Vector3D(0.0f, 1.0f, 0.0f));

}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
    if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
        return E_FAIL;
	}

    RECT rc;
    GetClientRect(_hWnd, &rc);
    _WindowWidth = rc.right - rc.left;
    _WindowHeight = rc.bottom - rc.top;

    if (FAILED(InitDevice()))
    {
        Cleanup();

        return E_FAIL;
    }

	// Initialize the world matrix
	XMStoreFloat4x4(&_world, XMMatrixIdentity());
	SetNumberOfAStroid();

    // Initialize the view matrix


	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	_pd3dDevice->CreateSamplerState(&sampDesc, &_pSamplerLinear);
	_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);

	return S_OK;

}



HRESULT Application::InitShadersAndInputLayout()
{
	HRESULT hr;


    // Compile the vertex shader
    ID3DBlob* pVSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "VS", "vs_4_0", &pVSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{	
		pVSBlob->Release();
        return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
    hr = CompileShaderFromFile(L"DX11 Framework.fx", "PS", "ps_4_0", &pPSBlob);

    if (FAILED(hr))
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return hr;
    }

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

    if (FAILED(hr))
        return hr;

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Normal", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
                                        pVSBlob->GetBufferSize(), &_pVertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
        return hr;

    // Set the input layout
    _pImmediateContext->IASetInputLayout(_pVertexLayout);

	return hr;
}

HRESULT Application::InitVertexBuffer()
{
	HRESULT hr;
	

	CreateD11Vertex(CubeVertices3D, 8);
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = tempVertexArr;

	hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pVertexBuffer);

	SimpleVertex pyramidVertices[] = {
		{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },//top point,
		{ XMFLOAT3(0.5f, -0.5f,- 0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f,-0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f,0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f,0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
	};



	D3D11_BUFFER_DESC psbd;
	ZeroMemory(&psbd, sizeof(psbd));
	psbd.Usage = D3D11_USAGE_DEFAULT;
	psbd.ByteWidth = sizeof(SimpleVertex) * 5;
	psbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	psbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA init;
	ZeroMemory(&init, sizeof(init));
	init.pSysMem = pyramidVertices;
	hr = _pd3dDevice->CreateBuffer(&psbd, &init, &_pPyramidVertexBuffer);


	SimpleVertex planeVertices[] = {
		//First Row
		{ XMFLOAT3(-0.4f, 0.0f, 0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },//top left,
		{ XMFLOAT3(-0.2f, 0.0f, 0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, 0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.2f, 0.0f, 0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.4f, 0.0f, 0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },

		//second row
		{ XMFLOAT3(-0.4f, 0.0f, 0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(-0.2f, 0.0f, 0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, 0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.2f, 0.0f, 0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.4f, 0.0f, 0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },

		//Third row
		{ XMFLOAT3(-0.4f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(-0.2f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.2f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.4f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },

		//4th row
		{ XMFLOAT3(-0.4f, 0.0f, -0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(-0.2f, 0.0f, -0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, -0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.2f, 0.0f, -0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.4f, 0.0f, -0.2f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },

		//5th row
		{ XMFLOAT3(-0.4f, 0.0f,-0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(-0.2f, 0.0f, -0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, -0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.2f, 0.0f, -0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },
		{ XMFLOAT3(0.4f, 0.0f, -0.4f), XMFLOAT4(1.0f, 0.0f, 0.5f, 1.0f) },

	};



	D3D11_BUFFER_DESC pbd;
	ZeroMemory(&pbd, sizeof(pbd));
	pbd.Usage = D3D11_USAGE_DEFAULT;
	pbd.ByteWidth = sizeof(SimpleVertex) * 25;
	pbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	pbd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initPlane;
	ZeroMemory(&initPlane, sizeof(initPlane));
	initPlane.pSysMem = planeVertices;
	hr = _pd3dDevice->CreateBuffer(&pbd, &initPlane, &_pPlaneVertexBuffer);


    if (FAILED(hr))
        return hr;

	return S_OK;
}

HRESULT Application::InitIndexBuffer()
{
	HRESULT hr;

    // Create index buffer
 
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;     
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = CubeIndices;
    hr = _pd3dDevice->CreateBuffer(&bd, &InitData, &_pIndexBuffer);


	// Create index buffer
	WORD pyramindIndices[] =
	{
		0,1,2,
		0,2,4,
		4,3,0,
		3,1,0,
		2,3,4,
		2,1,3

	};

	D3D11_BUFFER_DESC pyibd;
	ZeroMemory(&pyibd, sizeof(pyibd));

	pyibd.Usage = D3D11_USAGE_DEFAULT;
	pyibd.ByteWidth = sizeof(WORD) * 24;
	pyibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	pyibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA init;
	ZeroMemory(&init, sizeof(InitData));
	init.pSysMem = pyramindIndices;
	hr = _pd3dDevice->CreateBuffer(&pyibd, &init, &_pPyramidIndexBuffer);

	
	D3D11_BUFFER_DESC pibd;
	ZeroMemory(&pibd, sizeof(pibd));

	pibd.Usage = D3D11_USAGE_DEFAULT;
	pibd.ByteWidth = sizeof(WORD) * 96;
	pibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	pibd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initPlane;
	ZeroMemory(&initPlane, sizeof(initPlane));
	initPlane.pSysMem = PlaneIndicies;
	hr = _pd3dDevice->CreateBuffer(&pibd, &initPlane, &_pPlaneIndexBuffer);


	CreateDDSTextureFromFile(_pd3dDevice, defaultTexturePath, nullptr, &_pDefaultTextureRV);
	InitGameObject(Vector3D(), "Models/cylinder.obj", _pDefaultTextureRV);
	InitGameObject(Vector3D(0.f,2.0f,0.0f), "Models/cube.obj", _pDefaultTextureRV);
	InitPrimitiveGameObject(GameObjectPrimitives::PrimitiveType::Cube,Vector3D(0.f, -2.0f, 0.0f), _pDefaultTextureRV);

	UINT tstCount = testData.IndexCount;
    if (FAILED(hr))
        return hr;


	return S_OK;
}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = MsgSetUp;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
    if (!RegisterClassEx(&wcex))
        return E_FAIL;

    // Create window
    _hInst = hInstance;
    RECT rc = {0, 0, 640, 480};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    _hWnd = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                         nullptr);
    if (!_hWnd)
		return E_FAIL;

    ShowWindow(_hWnd, nCmdShow);

    return S_OK;
}

HRESULT Application::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* pErrorBlob;
    hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, 
        dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

    if (FAILED(hr))
    {
        if (pErrorBlob != nullptr)
            OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

        if (pErrorBlob) pErrorBlob->Release();

        return hr;
    }

    if (pErrorBlob) pErrorBlob->Release();

    return S_OK;
}

HRESULT Application::InitDevice()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;

#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	//Define swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = _WindowWidth;
    sd.BufferDesc.Height = _WindowHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = _hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;


	//Define stencil
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = _WindowWidth;
	depthStencilDesc.Height = _WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	
	

    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
        _driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
                                           D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
        if (SUCCEEDED(hr))
            break;
    }

    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    if (FAILED(hr))
        return hr;

    hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
    pBackBuffer->Release();

	//Create render stencil buffer
	hr = _pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);

	if (FAILED(hr))
		return hr;
	//Create render stencil view
	hr =_pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

	if (FAILED(hr))
		return hr;

    _pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)_WindowWidth;
    vp.Height = (FLOAT)_WindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    _pImmediateContext->RSSetViewports(1, &vp);

	InitShadersAndInputLayout();
	InitVertexBuffer();

	InitIndexBuffer();
	
	//donutMeshData =OBJLoader::Load("Models/donut.obj",_pd3dDevice);

    // Set primitive topology
    _pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);

	//Define wireframe state
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
	wfdesc.FillMode = D3D11_FILL_WIREFRAME;
	wfdesc.CullMode = D3D11_CULL_NONE;

	hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_wireFrame);

	//Define wireframe state
	D3D11_RASTERIZER_DESC sMdesc;
	ZeroMemory(&sMdesc, sizeof(D3D11_RASTERIZER_DESC));
	sMdesc.FillMode = D3D11_FILL_SOLID;
	sMdesc.CullMode = D3D11_CULL_BACK;

	hr = _pd3dDevice->CreateRasterizerState(&sMdesc, &_solidMode);


	if (FAILED(hr))
		return hr;

    if (FAILED(hr))
        return hr;

    return S_OK;
}

void Application::Cleanup()
{
    if (_pImmediateContext) _pImmediateContext->ClearState();
	if (_wireFrame) _wireFrame->Release();
	if (_depthStencilView) _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();
    if (_pConstantBuffer) _pConstantBuffer->Release();
    if (_pVertexBuffer) _pVertexBuffer->Release();
    if (_pIndexBuffer) _pIndexBuffer->Release();
    if (_pVertexLayout) _pVertexLayout->Release();
	if (_pPyramidIndexBuffer) _pPyramidIndexBuffer->Release();
	if (_pPyramidVertexBuffer) _pPyramidVertexBuffer->Release();
    if (_pVertexShader) _pVertexShader->Release();
    if (_pPixelShader) _pPixelShader->Release();
    if (_pRenderTargetView) _pRenderTargetView->Release();
    if (_pSwapChain) _pSwapChain->Release();
    if (_pImmediateContext) _pImmediateContext->Release();
    if (_pd3dDevice) _pd3dDevice->Release();

	CleanUpCameras();
	CleanUpGameObjects();
	
}

void Application::Update()
{
	_input->Update();
    // Update our time
    static float t = 0.0f;
	float r = 0.0f;
    if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float) XM_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();

        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;

        t = (dwTimeCur - dwTimeStart) / 1000.0f;

    }
	if (!cameras.empty()) {
		GetActiveCamera()->Update(t);
	}
	

	UpdateGameObjects(t);
	ScaleAndOffsetAstroids(t);
	gTime = t;
	lightDirection = XMFLOAT3(0.25f, 0.5f, -1.0f);
	// Diffuse material properties (RGBA)
	diffuseMaterial = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);
	// Diffuse light colour (RGBA)
	diffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//Ambient
	ambientMaterial = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	ambientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);

	//Specular
	specularMat = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	specularLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	eyePosW = XMFLOAT3(0.0f, 0.0f, 0.0f);
	specularPower = 10.0f;

}

void Application::Draw()
{
    //
    // Clear the back buffer
    //
    float ClearColor[4] = {0.0f, 0.125f, 0.3f, 1.0f}; // red,green,blue,alpha
    _pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);

	_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (isWireFrameModeOn) {

		_pImmediateContext->RSSetState(_wireFrame);
	}
	else {

		_pImmediateContext->RSSetState(_solidMode);
	}
	

	XMMATRIX world = XMLoadFloat4x4(&_world);
	XMMATRIX view = XMLoadFloat4x4(&GetActiveCamera()->GetViewXMMATRIX());
	XMMATRIX projection = XMLoadFloat4x4(&GetActiveCamera()->GetProjectionXMMATRIX());

    ConstantBuffer cb;


	cb.mWorld = XMMatrixTranspose(world);
	cb.mView = XMMatrixTranspose(view);
	cb.mProjection = XMMatrixTranspose(projection);
	cb.gTime = gTime;
	cb.DiffuseLight = diffuseLight;
	cb.DiffuseMtrl = diffuseMaterial;
	cb.LightVecW = lightDirection;
	cb.AmbientCol = ambientColor;
	cb.AmbientMtrl = ambientMaterial;
	cb.SpecularLight = specularLight;
	cb.SpecularMtrl = specularMat;
	cb.SpecularPower = specularPower;
	cb.EyePosW = eyePosW;

	//Use default texture as it is set on a per object basis

	_pTextureRV = _pDefaultTextureRV;
	_pImmediateContext->PSSetShaderResources(0, 1, &_pTextureRV);
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    //
    // Renders a triangle
    //
	_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
    _pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);



	// Set vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	DrawGameObjects(_pImmediateContext,cb);

	DrawAstroids(cb, _pImmediateContext);


    _pSwapChain->Present(0, 0);
}

void Application::ZoomActiveCamera(bool zoomIn)
{
	if (GetActiveCamera()->GetMovingForward()) {
		GetActiveCamera()->Move(zoomIn);
	}
	else {

		GetActiveCamera()->Zoom(zoomIn);
	}
}

void Application::StrafeActiveCamera(bool strafeRight)
{

	GetActiveCamera()->Strafe(strafeRight);
}

void Application::MoveActiveCamAlongY(bool up)
{
	GetActiveCamera()->MoveOnY(up);
}

void Application::OrbitCameraY(bool rotateRight)
{
	GetActiveCamera()->RotateYAxis(rotateRight);
}

void Application::OrbitCameraX(bool orbitUP)
{
	GetActiveCamera()->RotateXAxis(orbitUP);
}

void Application::ChangeCameraMode()
{
	GetActiveCamera()->ToggleCameraMode();
}

void Application::SetActiveCameraTargetGameObject()
{
	if (!GetActiveCamera()->GetLookIsLookingForward()) {
		GetActiveCamera()->SetLookAtGameObject(GetSelectedObject());
	}
}

void Application::MoveObjectForward()
{
	MoveActiveGameObject(Vector3D(0.0f, 0.0f, 1.0f));
}

void Application::MoveActiveGameObject(Vector3D direction)
{
	GetSelectedObject()->Move(direction, objectMoveSpeed);
}





//None FrameWork functions


void Application::SetNumberOfAStroid()
{
	int random = rand() % maxNumOfAstroids + minNumOfAstroids;
	
	for (int k = 0; k < random; k++) {

		GameObjectPrimitives* newAstroid = new GameObjectPrimitives(GameObjectPrimitives::PrimitiveType::Cube,
			Vector3D(), _pd3dDevice, _pDefaultTextureRV );
		_astroids.push_back(newAstroid);
	}
	for (int i = 0; i < _astroids.size(); i++) {

		float x = float(rand() % 100) * 0.1f;
		if (x == 0.0f) {
			x += float(rand() % 1 + 5) * 0.1f;
		}
		if (i % 2 > 0) {
			x = -x;
		}

		_astroidOffset.push_back(x);
		float s = float(rand() % 10) * 0.02f;
		if (s == 0.0f) {

			s += float(rand() % 2 + 1) * 0.1f;
		}
		_astroidScales.push_back(s);
		float v = float(rand() % 10) * 0.5f;
		if (v == 0.0f) {
			v += float(rand() % 5 + 1) * 0.1f;
		}
		_astroidRotationSpeedScalar.push_back(v);
	}

}

void Application::ScaleAndOffsetAstroids(float time)
{
	float t = time;


	for (int i = 0; i < _astroids.size(); i++) {

		_astroids[i]->SetPosition(Vector3D(_astroidOffset[i] * 1.25f, _astroidOffset[i] * 1.25f, 0.0f));
		_astroids[i]->SetLocalScale(Vector3D(_astroidScales[i], _astroidScales[i], _astroidScales[i]));
		_astroids[i]->SetRotation(Vector3D(0.0f,0.0f, -t ));
		_astroids[i]->SetOrbitSpeed(t*_astroidRotationSpeedScalar[i]/2.0f);
		_astroids[i]->Update(t);
	}

	
}

void Application::DrawAstroids(ConstantBuffer cb, ID3D11DeviceContext* deviceContext)
{
	
	for (GameObjectPrimitives* astroid : _astroids) {
		_pTextureRV = astroid->GetTexture();
		_pImmediateContext->PSSetShaderResources(0, 1, &_pTextureRV);
		cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&astroid->GetObjectWorldMatrix()));
		_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
		astroid->Draw(deviceContext);
	}
}

void Application::CreateD11Vertex(Vertex3D arr[],  int arrLength)
{
	tempVertexArr = new Vertex[arrLength];

	int triangleVertexCount =0;
	//int vertexIndex = 0;//start at index one  (second element)
	Vector3D vec1;
	Vector3D vec2;
	//Add all positions
	for (int i = 0; i< arrLength; i++) {
		//Pass to  position and UV to array
		tempVertexArr[i].Pos = XMFLOAT3(arr[i].Pos.show_X(), arr[i].Pos.show_Y(), arr[i].Pos.show_Z());
		tempVertexArr[i].TexC = arr[i].UV;
		//if it is the first vertex of the triangle 
		if (triangleVertexCount == 0) {
			vec1 = arr[i + 1].Pos - arr[i].Pos;//the index positions are both greater
			vec2 = arr[i + 2].Pos - arr[i].Pos;
			Vector3D cross = vec1.cross_product(vec2);
			Vector3D crossNormalised = cross.normalization();
			//Upload normal data
			tempVertexArr[i].Normal = XMFLOAT3(crossNormalised.show_X(), crossNormalised.show_Y(), crossNormalised.show_Z());
			triangleVertexCount++;
		//if it is the second vertex
		}else if(triangleVertexCount==1){
			vec1 = arr[i-1].Pos - arr[i].Pos;// go back to origin
			vec2 = arr[i + 2].Pos - arr[i].Pos;
			Vector3D cross = vec1.cross_product(vec2);
			Vector3D crossNormalised = cross.normalization();
			//Upload normal data
			tempVertexArr[i].Normal = XMFLOAT3(crossNormalised.show_X(), crossNormalised.show_Y(), crossNormalised.show_Z());
			triangleVertexCount++;
		}
		//3rd vertex
		else if (triangleVertexCount == 2) {
			vec1 = arr[i - 1].Pos - arr[i].Pos;//go to orign of triangle
			vec2 = arr[i - 2].Pos - arr[i].Pos;// go to previous vertex
			Vector3D cross = vec1.cross_product(vec2);
			Vector3D crossNormalised = cross.normalization();
			//Upload normal data
			tempVertexArr[i].Normal = XMFLOAT3(crossNormalised.show_X(), crossNormalised.show_Y(), crossNormalised.show_Z());

			float sumOfX = tempVertexArr[i].Normal.x + tempVertexArr[i-1].Normal.x + tempVertexArr[i-2].Normal.x;
			float sumOfY = tempVertexArr[i].Normal.y + tempVertexArr[i - 1].Normal.y+ tempVertexArr[i - 2].Normal.y;
			float sumOfZ = tempVertexArr[i].Normal.z + tempVertexArr[i - 1].Normal.z +tempVertexArr[i - 2].Normal.z;

			float  averageX = sumOfX / 3.0f;
			float  averageY = sumOfY/ 3.0f;
			float  averageZ = sumOfZ / 3.0f;

			tempVertexArr[i - 2].Normal = XMFLOAT3(averageX, averageY, averageZ);
			triangleVertexCount == 0;
		}

	}


	


}

void Application::ToggleWireFrame()
{
	if (isWireFrameModeOn) {
		isWireFrameModeOn = false;
	}
	else {
		isWireFrameModeOn = true;
	}
	
}


void Application::InitCamera(Vector3D initPos, Vector3D lootAt, Vector3D up)
{
	//Initialise a new camera
	Camera* newCam = new Camera(initPos, lootAt, up,_WindowWidth,_WindowHeight,_NearDepth,_FarDepth);
	//Add to vector of camera;
	cameras.push_back(newCam);
}
void Application::InitGameObject(Vector3D initPos, char* modelPath, ID3D11ShaderResourceView* texture)
{
	if (_pd3dDevice != NULL) {
		GameObject* newGameObject = new GameObject(modelPath, initPos, _pd3dDevice, texture);
		_gameObjects.push_back(newGameObject);
	}
}
void Application::InitPrimitiveGameObject(GameObjectPrimitives::PrimitiveType objectType, Vector3D initPos, ID3D11ShaderResourceView* texture)
{
	if (_pd3dDevice != NULL) {
		GameObjectPrimitives* newGameObject = new GameObjectPrimitives(objectType, initPos, _pd3dDevice, texture);
		_gameObjects.push_back(newGameObject);
	}
}
void Application::DrawGameObjects(ID3D11DeviceContext* deviceContext, ConstantBuffer cb)
{
	if (!_gameObjects.empty()) {

		for (GameObject*  object : _gameObjects) {
	
			_pTextureRV = object->GetTexture();
			_pImmediateContext->PSSetShaderResources(0, 1, &_pTextureRV);
			cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&object->GetObjectWorldMatrix()));
			_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
			object->Draw(deviceContext);
		}
			
	}
}
void Application::UpdateGameObjects(float time)
{
	for (GameObject* object : _gameObjects) {
		object->Update(time);
	}
}
void Application::CleanUpCameras()
{
	for each (Camera * cam in cameras) {
		delete cam;
		cam = nullptr;
	}
	cameras.clear();
}
void Application::CleanUpGameObjects()
{
	for each (GameObject * objects in _gameObjects) {

		objects = nullptr;
	}
	_gameObjects.clear();
}
void Application::CycleBetweenCameras()
{

	if (activeCamIndex >= cameras.size()-1) {
		activeCamIndex = 0;
	}
	else {
		activeCamIndex++;
	}
}

void Application::CycleBetweenGameObjects()
{
	if (selectedGameObjectIndex >= _gameObjects.size() - 1) {
		selectedGameObjectIndex = 0;
	}
	else {
		selectedGameObjectIndex++;
	}
}

void Application::SetActiveCamera(int index)
{
	if (index <= cameras.size()-1) {
		activeCamIndex = index;
	}
	else {
		activeCamIndex = 0;
	}
}

Camera* Application::GetActiveCamera()
{
	return cameras[activeCamIndex];
}

GameObject* Application::GetSelectedObject()
{
	return _gameObjects[selectedGameObjectIndex];
}

//=========================================WINDOWS PROCS================================================
LRESULT CALLBACK  Application::MsgSetUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Application* const pApp = static_cast<Application*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Application::HandleMsgThunk));

		return pApp->HandleMsgThunk(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK  Application::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application* const pApp =reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pApp->MsgHandler(hWnd, msg, wParam, lParam);
}

LRESULT Application::MsgHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	//=====================Keyboard Events==================================

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		break;

	case WM_KEYDOWN:

	case WM_SYSKEYDOWN:
	
	
	case WM_KEYUP:


		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//============================================================================================



