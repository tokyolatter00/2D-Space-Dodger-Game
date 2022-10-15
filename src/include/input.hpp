
#pragma once

#define KEY_W					'W'
#define KEY_A					'A'
#define KEY_S					'S'
#define KEY_D					'D'
#define KEY_C					'C'
#define KEY_SPACE				VK_SPACE
#define KEY_SHIFT				VK_SHIFT
#define KEY_NULL				0

#define BUTTON_LEFT				1000

class InputHandler {
private:
	unsigned long LastKey;
	unsigned long PressedKey;
	unsigned long HeldKey;
public:
	InputHandler();

	void RegisterKeyDown(unsigned long key);
	void RegisterKeyUp(unsigned long key);
	bool KeyIsPressed(unsigned long key);
	bool KeyIsHeld(unsigned long key);
	bool KeyIsPressedOrHeld(unsigned long key);
};
