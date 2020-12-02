#pragma once
#include <queue>
#include <iostream>
#include <bitset>

class Keyboard {
	friend class Application;

	//Public classes
public:
	//Member class to store and handle different types of key events
	class Event {
	public:
		enum class Type
		{
			Down,
			Up,
			Invalid
		};


	private:
		Type eventType;
		unsigned char code;

	public:
		//Default constructer
		Event()
			:
			eventType(Type::Invalid),
			code(0u)
		{}

		//Specified constructor
		Event(Type typeOfKeyEvent, unsigned char keyCode)
			:
			//If event type has been specified it creates a respective event
			eventType(typeOfKeyEvent),
			code(keyCode)
		{}
		bool IsDown() {
			return eventType == Type::Down; //returns true if event is a down event
		}
		bool IsUp() {
			return eventType == Type::Up; //returns true if event is a up event
		}
		bool IsValid() {
			return eventType != Type::Invalid; //returns true if event is a up event
		}
		//Returns the key code of event
		unsigned char GetKeyCode() {
			return code;
		}
	};


	//Public functions - Allows client to interface with application
public:

	//Construction
	Keyboard() = default;

	//Deletion
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	//Keyevent handlers
	bool GetKeyDown(unsigned char keyCode) const noexcept; //Returns true if key is down
	Event ReadKey()  noexcept; //Pulls key event off event queue
	bool IsKeyQueueEmpty(); // checks if there are any key events in queue
	void ClearKeyQueue(); //Clears key events queue

	//Char event handlers

	char ReadChar(); //Gets character of current key in queue
	bool IsCharEmpty(); //Checks  if there is a char in current key queue
	void ClearChar(); //Clears current char value from queue
	void Clear(); //Clears both key event and char event queue


//private functions - Inaccessible true client but as application is a friend it can access them
private:
	void OnKeyDown(unsigned char keyCode); //When a key is pressed
	void OnKeyUp(unsigned char keyCode); //when a key is released
	void OnChar(char character); //When a character value is passed in
	void ClearState(); //Clears all key sets held by bit set
	bool AutoRepeatIsOn();
	void EnableAutoRepeat();
	void DisableAutoRepeat();
	//Buffer manager
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;//Reduces buffer if it exceeds set buffer size (helps limit the size of the queue)

private:
	static constexpr unsigned int nKeys = 256u; //max amount of virtual key sets
	static constexpr unsigned int bufferSize = 16u;//Max size of buffer
	bool autoRepeatOn = false;
	std::bitset<nKeys> keyStates;
	//std::ctype_base::mask keyStates = nKeys;
	std::queue<Event> keyBuffer;//queue of key events
	std::queue<char> charBuffer; //queue of char events

};


