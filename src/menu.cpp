
#include <sstream>
#include "include/levels/menu.hpp"

bool Menu::Load() {
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

	// Load UI - Title Label
	
	args.Foreground = Colour32White;
	args.Background = Colour32Transparent;
	args.Rect = Mat4(200.0f, 110.0f, 250.0f, 50.0f);

	TitleLabel = new Label(args);

	TitleLabel->SetText(
		Gfx,
		L"2D SPACE DODGER",
		Gfx->GetTextFormat(FontTitle)
	);

	// Load UI - Start Button

	args.Foreground = Colour32White;
	args.Background = Colour32DarkGray;
	args.Border = BorderType::ROUNDED;
	args.Rect = Mat4(200.0f, 230.0f, 250.0f, 50.0f);

	StartButton = new Button(args);

	StartButton->SetText(
		Gfx,
		L"START",
		Gfx->GetTextFormat(FontLight)
	);

	return true;
}

void Menu::Unload() {
	Gfx->ClearTextFormats();

	delete Input;
	delete Border;
	delete TitleLabel;
	delete StartButton;
}

void Menu::Update(float timeTotal, float timeDelta) {
	// Check if left click was pressed

	if (Input->KeyIsPressed(BUTTON_LEFT)) {
		// Check if user has clicked start

		if (StartButton->HoveringOver(*CursorPos)) {
			*Result = new LevelResult(LevelExitCode::START_GAME);
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

void Menu::Draw() {
	// Draw UI

	Border->Draw(Gfx);
	TitleLabel->Draw(Gfx);
	StartButton->Draw(Gfx);
}

void Menu::ProcessKey(UINT uMsg, WPARAM key) {
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

void Menu::ProcessMouse(UINT button) {
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
