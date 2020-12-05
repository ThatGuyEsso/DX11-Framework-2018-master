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
			if (theApp->_input->GetKey(0x51)) {
				theApp->ChangeCameraMode();
			}
			if (theApp->_input->GetKey(VK_SPACE)) {
				theApp->ToggleWireFrame();
			}
			if (theApp->_input->GetKey(0x43)) {
				theApp->CycleBetweenCameras();
			}

			if (theApp->_input->GetKey(0x31)) {
				theApp->SetActiveCamera(0);
			}
			if (theApp->_input->GetKey(0x32)) {
				theApp->SetActiveCamera(1);
			}
			if (theApp->_input->GetKey(0x33)) {
				theApp->SetActiveCamera(2);
			}
			if (theApp->_input->GetKey(VK_UP) ) {
				theApp->ZoomActiveCamera(true);
			}
			else if (theApp->_input->GetKey(VK_DOWN)){
				theApp->ZoomActiveCamera(false);
			}

			if (theApp->_input->GetKey(0x46)) {
				theApp->OrbitCameraX(true);
			}
			else if (theApp->_input->GetKey(0x44))
			{
				theApp->OrbitCameraX(false);
			}
			if (theApp->_input->GetKey(VK_RIGHT)) {
				theApp->OrbitCameraY(true);
				theApp->StrafeActiveCamera(true);
			}
			else if (theApp->_input->GetKey(VK_LEFT)) {
				theApp->OrbitCameraY(false);
				theApp->StrafeActiveCamera(false);
			}
			if (theApp->_input->GetKey(VK_OEM_PLUS)) {
				theApp->MoveActiveCamAlongY(true);
			}
			else if (theApp->_input->GetKey(VK_OEM_MINUS)) {
				theApp->MoveActiveCamAlongY(false);
			}

			if (theApp->_input->GetKey(VK_SHIFT)){
				theApp->MoveObjectForward();
			}
			if (theApp->_input->GetKey(VK_CONTROL)) {
				theApp->CycleBetweenGameObjects();
			}

		}
	
		theApp->Update();
        theApp->Draw();

    }

	delete theApp;
	theApp = nullptr;

    return (int) msg.wParam;
}