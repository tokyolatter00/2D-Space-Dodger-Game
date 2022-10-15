
#include "include/level.hpp"

// Level Result

LevelResult::LevelResult() {
	Code = LevelExitCode::UNDEFINED;
}

LevelResult::LevelResult(LevelExitCode code) {
	Code = code;
}

LevelResult::LevelResult(LevelExitCode code, UINT _uint) {
	Code = code;
	Value._uint = _uint;
}

// Level

Graphics* Level::Gfx;
LevelType Level::Type;
HWND Level::HWnd;
UINT* Level::FPS;
Mat2* Level::CursorPos;
LevelResult** Level::Result;

void Level::Init(Graphics* gfx, HWND hWnd, UINT* fps, Mat2* cursorPos, LevelResult** res) {
	Gfx = gfx;
	HWnd = hWnd;
	FPS = fps;
	CursorPos = cursorPos;
	Result = res;
}
