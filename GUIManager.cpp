#include "GUIManager.h"


GUIManager::GUIManager()
{
	//set up IMGUI	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();



}

GUIManager::~GUIManager()
{
	//destroy

	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void GUIManager::Init(void* hWnd,ID3D11Device* device, ID3D11DeviceContext* pDeviceContext)
{
	//initialise IMGUI
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(device, pDeviceContext);

	//ImGui::Begin()

}

