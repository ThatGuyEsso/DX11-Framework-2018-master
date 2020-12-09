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

void GUIManager::DrawGUI()
{



	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	static bool ShowDemoWindow = true;
	if (ShowDemoWindow) {
		ImGui::ShowDemoWindow(&ShowDemoWindow);
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
