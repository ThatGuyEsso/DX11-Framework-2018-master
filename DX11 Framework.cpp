#include "Application.h"

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	Application * theApp = new Application();

	if (FAILED(theApp->Initialise(hInstance, nCmdShow)))
	{
		return -1;
	}

    // Main message loop
    MSG msg = {0};
	BOOL gResult;

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			//Q Change camera mode (look at targte to loot to target)
			if (theApp->_input->GetKey(0x51)) {
				theApp->ChangeCameraMode();
			}
			//Space toggle wireframe mode
			if (theApp->_input->GetKey(VK_SPACE)) {
				theApp->ToggleWireFrame();
			}
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

			if (theApp->_input->GetKey(0x47)){
				theApp->MoveObjectForward();
			}
			//G cycle between Game objects
			if (theApp->_input->GetKey(0x47)) {
				theApp->CycleBetweenGameObjects();
			}
			//B Bind Camera to gameobject
			if (theApp->_input->GetKey(0x42)) {
				theApp->SetActiveCameraTargetGameObject();
			}

			if (theApp->_input->GetKey(VK_END)) {
				theApp->ActiveCameraStartPath();
			}
		}
	
		theApp->Update();
        theApp->Draw();

    }

	delete theApp;
	theApp = nullptr;

    return (int) msg.wParam;
}