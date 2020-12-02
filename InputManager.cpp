#include "InputManager.h"

bool InputManager::GetKey(byte key)
{
	if (GetAsyncKeyState(key) & 0x0001) {
		return true;
	}
	else {
		return false;
	}
}

void InputManager::Update()
{
	GetKeyboardState(keys);
	
}