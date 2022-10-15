
#pragma once

#include "level.hpp"
#include "cputimer.hpp"

class LevelController {
private:
	static CpuTimer* Timer;
	static Level* ActiveLevel;
public:
	static void Init();
	static void Release();

	static void LoadInitialLevel(Level* level);
	static void SwitchLevel(Level* level);

	static void ProcessKey(UINT uMsg, WPARAM key);
	static void ProcessMouse(UINT button);

	static void Update();
	static void Draw();
};
