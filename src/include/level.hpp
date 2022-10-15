
#pragma once

#include "engine.hpp"
#include "sprites.hpp"
#include "ui.hpp"

/*
	LEVEL_TYPE_MENU				- Menu Level
	LEVEL_TYPE_GAME				- Game Level
	LEVEL_TYPE_GAME_OVER		- Game Over Level
*/

enum class LevelType {
	LEVEL_TYPE_MENU,
	LEVEL_TYPE_GAME,
	LEVEL_TYPE_GAME_OVER
};

/*
	UNDEFINED					- None
	START_GAME					- Start Game Level
	START_GAME_OVER				- Start Game Over Level
	EXIT_APP					- Exit Application
*/

enum class LevelExitCode {
	UNDEFINED,
	START_GAME,
	START_GAME_OVER,
	EXIT_APP
};

struct LevelResult {
	LevelExitCode Code;

	union {
		UINT _uint;
	} Value;

	LevelResult();
	LevelResult(LevelExitCode code);
	LevelResult(LevelExitCode code, UINT _uint);
};

class Level {
protected:
	static Graphics* Gfx;
	static HWND HWnd;
	static UINT* FPS;
	static Mat2* CursorPos;
	static LevelResult** Result;
public:
	static LevelType Type;

	static void Init(Graphics* gfx, HWND hWnd, UINT* fps, Mat2* cursorPos, LevelResult** res);
	
	virtual bool Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(float timeTotal, float timeDelta) = 0;
	virtual void Draw() = 0;

	virtual void ProcessKey(UINT uMsg, WPARAM key) = 0;
	virtual void ProcessMouse(UINT button) = 0;
};
