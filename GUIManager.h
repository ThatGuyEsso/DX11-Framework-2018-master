#pragma once
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_win32.h"
#include "vendor/imgui/imgui_impl_dx11.h"
class GUIManager {

public:
	GUIManager();
	~GUIManager();
	void Init(void* hWnd, ID3D11Device* device, ID3D11DeviceContext* pDeviceContext);
	void DrawGUI();
};