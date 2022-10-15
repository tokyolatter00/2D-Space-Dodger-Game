
#include <Windows.h>
#include "include/input.hpp"

InputHandler::InputHandler() {
	LastKey = KEY_NULL;
	HeldKey = KEY_NULL;
}

void InputHandler::RegisterKeyDown(unsigned long key) {
	// Register a key down message

	if (LastKey == key) {
		HeldKey = key;
	}
	else {
		LastKey = key;
		HeldKey = KEY_NULL;
	}
}

void InputHandler::RegisterKeyUp(unsigned long key) {
	// Register a key up message

	PressedKey = key;
	LastKey = KEY_NULL;
	HeldKey = KEY_NULL;
}

bool InputHandler::KeyIsPressed(unsigned long key) {
	// Check if a key has been pressed

	if (PressedKey == key) {
		PressedKey = KEY_NULL;
		return true;
	}

	return false;
}

bool InputHandler::KeyIsHeld(unsigned long key) {
	// Check if a key is being held

	if (HeldKey == key) {
		HeldKey = NULL;
		return true;
	}

	return false;
}

bool InputHandler::KeyIsPressedOrHeld(unsigned long key) {
	// Check if a key has been pressed or is being held

	return KeyIsPressed(key) || KeyIsHeld(key);
}
