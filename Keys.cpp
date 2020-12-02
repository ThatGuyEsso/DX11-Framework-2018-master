#include "Keys.h"

bool Keys::GetKeyDown(unsigned int keyCode)
{
	if (heldKeys.empty()) {
		for each (unsigned int key in heldKeys)
		{
			if (key == keyCode) return true;
		}
		return false;
	}
	else {
		return false;
	}
}

bool Keys::AnyKeysHeld()
{
	if (!heldKeys.empty()) {
		return true;
	}
	else {
		return false;
	}
}

void Keys::OnKeyDown(unsigned int keyCode)
{
	heldKeys.push_back(keyCode);
	if (AnyKeysHeld()) {
		TrimBuffer(heldKeys);
	}
}

void Keys::OnKeyUp(unsigned int keyCode)
{
	if (AnyKeysHeld()) {
		std::vector<int> deletionIndices;
		for (int i = 0; i < heldKeys.size(); i++) {
			if (heldKeys[i] == keyCode)deletionIndices.push_back(i);
		}

		for (int i = 0; i < deletionIndices.size(); i++) {
			heldKeys.erase(heldKeys.begin() + deletionIndices[i]);
		}
	}

}
bool Keys::AutoRepeatIsOn()
{
	return autoRepeatOn;
}
void Keys::EnableAutoRepeat()
{
	autoRepeatOn = true;
}
void Keys::DisableAutoRepeat()
{
	autoRepeatOn = false;
}
void Keys::ClearKeys()
{
	heldKeys.clear();
}


template<typename T>
void Keys::TrimBuffer(std::vector<T>& buffer) noexcept
{
	while (buffer.size() > bufferSize) {
		buffer.pop_back();
	}
}