#pragma once
#include <vector>
class Keys {
	friend class Application;

public:

	//Construction
	Keys() = default;

	//Deletion
	Keys(const Keys&) = delete;
	Keys& operator=(const Keys&) = delete;
	bool GetKeyDown(unsigned int keyCode);
	bool AnyKeysHeld(); // checks if there are any key events in queue



private:
	void OnKeyDown(unsigned int keyCode); //When a key is pressed
	void OnKeyUp(unsigned int keyCode); //when a key is released
	bool AutoRepeatIsOn();
	void EnableAutoRepeat();
	void DisableAutoRepeat();
	void ClearKeys();
	template<typename T>
	static void TrimBuffer(std::vector<T>& buffer) noexcept;//Reduces buffer if it exceeds set buffer size (helps limit the size of the queue)


private:
	static constexpr unsigned int bufferSize = 16u;//Max size of buffer
	bool autoRepeatOn = false;
	std::vector<unsigned int> heldKeys;
};


