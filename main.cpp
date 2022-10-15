
#include "src/include/engine.hpp"
#include "src/include/controller.hpp"
#include "src/include/levels/game.hpp"
#include "src/include/levels/game-over.hpp"
#include "src/include/levels/menu.hpp"

Mat2 CursorPos;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_DESTROY:
			// Close Application

			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
			// Key Press

			LevelController::ProcessKey(uMsg, wParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			// Mouse Press

			LevelController::ProcessMouse(uMsg);
			break;
		case WM_MOUSEMOVE:
			// Mouse Move

			CursorPos.SetX(LOWORD(lParam));
			CursorPos.SetY(HIWORD(lParam));
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmd, int nCmdShow) {
	LPCWSTR className = L"Main Window";
	LPCWSTR title = L"Direct 2D Window";

	// Create Window Class

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = className;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);

	// Create Window

	RECT rect = { 0, 0, 640, 480 };

	AdjustWindowRectEx(
		&rect,
		WS_OVERLAPPEDWINDOW,
		FALSE,
		WS_EX_OVERLAPPEDWINDOW
	);
	
	int clientWidth = rect.right - rect.left;
	int clientHeight = rect.bottom - rect.top;

	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		className, title,
		WS_OVERLAPPEDWINDOW,
		100, 100, clientWidth, clientHeight,
		NULL, NULL, hInstance, NULL
	);

	if (!hWnd) {
		OutputDebugString(L"Initialising the hWnd failed\n");
		return -1;
	}

	// Create Graphics
	
	Graphics* graphics = new Graphics();
	
	if (!graphics->Init(hWnd)) {
		OutputDebugString(L"Initialising the Graphics failed\n");
		delete graphics;
		return -1;
	}

	// Display Window

	ShowWindow(hWnd, nCmdShow);

	// Initialise Classes

	Menu* menu = new Menu();
	Game* game = new Game();
	GameOver* game_over = new GameOver();

	// Shared Variables

	UINT fps = 0;
	LevelResult* result = nullptr;
	bool should_exit = false;

	// Load Levels

	Level::Init(graphics, hWnd, &fps, &CursorPos, &result);

	LevelController::Init();
	LevelController::LoadInitialLevel(menu);

	// Message Event Loop

	MSG msg;
	msg.message = WM_NULL;

	Stopwatch* timer = new Stopwatch();
	UINT _fps = 0;
	
	while (msg.message != WM_QUIT && !should_exit) {
		// Process Window Message

		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			DispatchMessage(&msg);
			continue;
		}

		// Update Game

		LevelController::Update();

		// Check Level Exit Code

		if (result) {
			switch (result->Code) {
				case LevelExitCode::START_GAME:
					// Start Game Level

					if (game) {
						game->Unload();
						delete game;
						game = nullptr;
					}

					game = new Game();
					LevelController::SwitchLevel(game);
					break;
				case LevelExitCode::START_GAME_OVER:
					// Start GameOver Level

					game_over->PlayerScore = result->Value._uint;

					LevelController::SwitchLevel(game_over);
					break;
				case LevelExitCode::EXIT_APP:
					// Exit Application

					should_exit = true;
					break;
			}

			delete result;
			result = nullptr;
		}

		// Render Game

		graphics->BeginDraw();
		graphics->ClearScreen(Colour32DarkGray);
		LevelController::Draw();
		graphics->EndDraw();

		// Measure FPS

		_fps++;

		if (timer->GetElapsed() > MILLISECONDS(1000)) {
			timer->Restart();
			fps = _fps;
			_fps = 0;
		}
	}

	// Memory Cleanup

	if (result) {
		delete result;
	}

	if (menu) {
		menu->Unload();
		delete menu;
	}

	if (game) {
		//game->Unload();
		delete game;
	}

	if (game_over) {
		game_over->Unload();
		delete game_over;
	}

	LevelController::Release();
	delete graphics;
	delete timer;
}
