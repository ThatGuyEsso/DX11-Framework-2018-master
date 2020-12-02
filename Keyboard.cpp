#include "Keyboard.h"

//Client Key events
//================================================================================
bool Keyboard::GetKeyDown(unsigned char keyCode) const noexcept
{
	return keyStates[keyCode];
	//return keyStates[&keyCode];//Return respective state of this key
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	//if there is a key in the queue
	if (keyBuffer.size() > 0u) {
		Keyboard::Event e = keyBuffer.front(); //the current event is equal to the first (first in first out)
		keyBuffer.pop(); //removes current event from queue
		return e; //return this event
	}
	else {
		return Keyboard::Event();
	}
}

bool Keyboard::IsKeyQueueEmpty()
{
	return keyBuffer.empty();// returns true if a queue is empty
}

void Keyboard::ClearKeyQueue()
{
	keyBuffer = std::queue<Event>();//constructs new empty char queue
}
//================================================================================
////Client Char events
//================================================================================
char Keyboard::ReadChar()
{
	//if there is a char in the queue
	if (charBuffer.size() > 0u) {
		unsigned int code = charBuffer.front(); //the current c haris equal to the first (first in first out)
		charBuffer.pop(); //removes current event from queue
		return code; //return this code
	}
	return{};
}

bool Keyboard::IsCharEmpty()
{
	return charBuffer.empty();
}

void Keyboard::ClearChar()
{

	charBuffer = std::queue<char>();//constructs new empty char queue
}

//================================================================================
//Queue Management
void Keyboard::Clear()
{
	ClearChar();
	ClearKeyQueue();
}

template<typename T>
static void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{

	//Pop of new values until queue fits max size
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}

void Keyboard::ClearState()
{
	//keyStates

}

bool Keyboard::AutoRepeatIsOn()
{
	return autoRepeatOn;
}

void Keyboard::EnableAutoRepeat()
{
	autoRepeatOn = true;
}

void Keyboard::DisableAutoRepeat()
{
	autoRepeatOn = false;
}

//================================================================================

//Keyboard Events
//================================================================================
void Keyboard::OnKeyDown(unsigned char keyCode)
{
	//If a key is held down
	keyStates[keyCode] = true;

	//Create event of respective event type and with key code and push it to queue
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Down, keyCode));

	//reduce buffer
	TrimBuffer(keyBuffer);
}
void Keyboard::OnKeyUp(unsigned char keyCode)
{
	//If a key is released 
	keyStates[keyCode] = false;

	//Create event of respective event type and with key code and push it to queue
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Up, keyCode));

	//reduce buffer
	TrimBuffer(keyBuffer);
}
void Keyboard::OnChar(char character)
{
	//Push current character code to char array
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}


//================================================================================