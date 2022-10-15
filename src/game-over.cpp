
#include <sstream>
#include "include/levels/game-over.hpp"

GameOver::GameOver() {
	PlayerScore = 0;
}

bool GameOver::Load() {
	// Create Objects

	Input = new InputHandler();

	// Create Fonts

	FontLight = Gfx->CreateTextFormat(
		L"Lucida Console",
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15.0f
	);

	FontTitle = Gfx->CreateTextFormat(
		L"Lucida Console",
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_OBLIQUE,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f
	);

	// Load UI

	ControlArgs args;

	// Load UI - Border

	args.Colour = Colour32LightGray;
	args.Border = BorderType::ROUNDED;
	args.Rect = Mat4(25.0f, 25.0f, 595.0f, 430.0f);

	Border = new Panel(args);

	// Load UI - Game Over Label

	args.Foreground = Colour32White;
	args.Background = Colour32Transparent;
	args.Rect = Mat4(200.0f, 110.0f, 250.0f, 50.0f);

	GameOverLabel = new Label(args);

	GameOverLabel->SetText(
		Gfx,
		L"GAME OVER",
		Gfx->GetTextFormat(FontTitle)
	);

	// Load UI - Score Output Label

	args.Foreground = Colour32White;
	args.Background = Colour32Transparent;
	args.Rect = Mat4(200.0f, 150.0f, 250.0f, 50.0f);

	ScoreOutputLabel = new Label(args);

	// Load UI - Try Again Button

	args.Foreground = Colour32Black;
	args.Background = Colour32White;
	args.Border = BorderType::ROUNDED;
	args.Rect = Mat4(200.0f, 230.0f, 250.0f, 50.0f);

	TryAgainButton = new Button(args);

	TryAgainButton->SetText(
		Gfx,
		L"TRY AGAIN",
		Gfx->GetTextFormat(FontLight)
	);

	// Load UI - Exit Button

	args.Foreground = Colour32Black;
	args.Background = Colour32White;
	args.Border = BorderType::ROUNDED;
	args.Rect = Mat4(200.0f, 300.0f, 250.0f, 50.0f);

	ExitButton = new Button(args);

	ExitButton->SetText(
		Gfx,
		L"EXIT",
		Gfx->GetTextFormat(FontLight)
	);

	return true;
}

void GameOver::Unload() {
	Gfx->ClearTextFormats();

	delete Input;
	delete Border;
	delete GameOverLabel;
	delete ScoreOutputLabel;
	delete TryAgainButton;
	delete ExitButton;
}

void GameOver::Update(float timeTotal, float timeDelta) {
	// Check if left click was pressed

	if (Input->KeyIsPressed(BUTTON_LEFT)) {
		// Check if user has clicked try again

		if (TryAgainButton->HoveringOver(*CursorPos)) {
			*Result = new LevelResult(LevelExitCode::START_GAME);
			return;
		}

		// Check if user has clicked exit

		if (ExitButton->HoveringOver(*CursorPos)) {
			*Result = new LevelResult(LevelExitCode::EXIT_APP);
			return;
		}
	}

	// Update Title

	std::wstringstream wss;
	wss << "Direct 2D Window";
	wss << " | ";
	wss << std::to_wstring(*FPS);
	wss << " FPS";

	SetWindowText(HWnd, wss.str().c_str());
}

void GameOver::Draw() {
	// Draw UI

	std::wstringstream wss;
	wss << "You finished with a score of ";
	wss << std::to_wstring(PlayerScore);

	ScoreOutputLabel->SetText(
		Gfx,
		wss.str(),
		Gfx->GetTextFormat(FontLight)
	);

	Border->Draw(Gfx);
	GameOverLabel->Draw(Gfx);
	ScoreOutputLabel->Draw(Gfx);
	TryAgainButton->Draw(Gfx);
	ExitButton->Draw(Gfx);
}

void GameOver::ProcessKey(UINT uMsg, WPARAM key) {
	// Process Key Message

	switch (uMsg) {
		case WM_KEYDOWN:
			Input->RegisterKeyDown(key);
			break;
		case WM_KEYUP:
			Input->RegisterKeyUp(key);
			break;
	}
}

void GameOver::ProcessMouse(UINT button) {
	// Process Mouse Message

	switch (button) {
		case WM_LBUTTONDOWN:
			Input->RegisterKeyDown(BUTTON_LEFT);
			break;
		case WM_LBUTTONUP:
			Input->RegisterKeyUp(BUTTON_LEFT);
			break;
	}
}

