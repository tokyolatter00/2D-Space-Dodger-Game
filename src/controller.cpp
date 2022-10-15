
#include "include/controller.hpp"

CpuTimer* LevelController::Timer;
Level* LevelController::ActiveLevel;

void LevelController::Init() {
	Timer = new CpuTimer();
	ActiveLevel = nullptr;
}

void LevelController::Release() {
	delete Timer;
}

void LevelController::LoadInitialLevel(Level* level) {
	// Load the First Level

	ActiveLevel = level;
	ActiveLevel->Load();
}

void LevelController::SwitchLevel(Level* level) {
	// Change Active Level

	ActiveLevel = level;

	if (!ActiveLevel->Load()) {
		OutputDebugString(L"Failed to Initialise Level\n");
		while (true);
	}
}

void LevelController::ProcessKey(UINT uMsg, WPARAM key) {
	// Process Key Message

	ActiveLevel->ProcessKey(uMsg, key);
}

void LevelController::ProcessMouse(UINT button) {
	// Process Mouse Message

	ActiveLevel->ProcessMouse(button);
}

void LevelController::Update() {
	// Update Active Level

	Timer->Update();

	ActiveLevel->Update(
		Timer->GetTimeTotal(),
		Timer->GetTimeDelta()
	);
}

void LevelController::Draw() {
	// Draw Active Level

	ActiveLevel->Draw();
}
