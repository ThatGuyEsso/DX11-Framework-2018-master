#include "Application.h"

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Application * theApp = new Application();//initialise new app

	//init app if fails return out
	if (FAILED(theApp->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}

    // Main message loop
    MSG msg = {0};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(!theApp->_guiManager->IsGuiTyping()) {

				//Space toggle wireframe mode
				if (theApp->_input->GetKey(VK_SHIFT)) {
					theApp->ToggleWireFrame();
				}
	//=====================Camera Controls=============================
				//C cycle between Cameras
				if (theApp->_input->GetKey(0x43)) {
					theApp->CycleBetweenCameras();
				}

				//alpha key1
				if (theApp->_input->GetKey(0x31)) {
					theApp->SetActiveCamera(0);
				}
				//alpha key2
				if (theApp->_input->GetKey(0x32)) {
					theApp->SetActiveCamera(1);
				}
				//alpha key3
				if (theApp->_input->GetKey(0x33)) {
					theApp->SetActiveCamera(2);
				}
				//Q Change camera mode (look at target to loot to target)
				if (theApp->_input->GetKey(0x51)) {
					theApp->ChangeCameraMode();
				}
				//Start Camera Path
				if (theApp->_input->GetKey(VK_CONTROL)) {
					theApp->ActiveCameraStartPath();
				}
				//up
				if (theApp->_input->GetKey(VK_UP) ) {
					theApp->ZoomActiveCamera(true);
				}
				//down
				else if (theApp->_input->GetKey(VK_DOWN)){
					theApp->ZoomActiveCamera(false);
				}
				//Right Strafe Right
				if (theApp->_input->GetKey(VK_RIGHT)) {
			
					theApp->StrafeActiveCamera(true);
				}
				//Left Strafe Left
				else if (theApp->_input->GetKey(VK_LEFT)) {

	
					theApp->StrafeActiveCamera(false);
				}
				//W orbit/rotate up
				if (theApp->_input->GetKey(0x57)) {
					theApp->OrbitCameraX(true);
					theApp->RotateCameraPitch(true);
				}
				//S orbit/rotate S
				else if (theApp->_input->GetKey(0x53))
				{
					theApp->OrbitCameraX(false);
					theApp->RotateCameraPitch(false);
				}
				//D orbit/rotate Right
				if (theApp->_input->GetKey(0x44)) {
					theApp->OrbitCameraY(true);
					theApp->RotateCameraYaw(true);
				}
				//A orbit/rotate Right
				else if (theApp->_input->GetKey(0x41)) {
					theApp->OrbitCameraY(false);
					theApp->RotateCameraYaw(false);
				}
				//+ Move camera Up 
				if (theApp->_input->GetKey(VK_OEM_PLUS)) {
					theApp->MoveActiveCamAlongY(true);
				}
				//+ Move camera Down 
				else if (theApp->_input->GetKey(VK_OEM_MINUS)) {
					theApp->MoveActiveCamAlongY(false);
				}
				//B Bind Camera to gameobject
				if (theApp->_input->GetKey(0x42)) {
					theApp->SetActiveCameraTargetGameObject();
				}
	//=======================================================================
	//====================GameObject Controls================================
				//G cycle between Game objects
				if (theApp->_input->GetKey(0x47)) {
					theApp->CycleBetweenGameObjects();
				}
				//Move object forward
				if (theApp->_input->GetKey(0x49)){
					theApp->MoveObjectForward();
				}
				//Move object backwards
				if (theApp->_input->GetKey(0x4B)) {
					theApp->MoveObjectBackwards();
				}
				//move object left
				if (theApp->_input->GetKey(0x4A)) {
					theApp->MoveObjectLeft();
				}
				//move object right
				if (theApp->_input->GetKey(0x4C)) {
					theApp->MoveObjectRight();
				}
//=======================================================================
				//F cycle between scene lights
				if (theApp->_input->GetKey(0x46)) {
					theApp->CycleBetweenLights();
				}
			}
		}
	
		theApp->Update();
        theApp->Draw();

    }

	delete theApp;
	theApp = nullptr;

    return (int) msg.wParam;
}