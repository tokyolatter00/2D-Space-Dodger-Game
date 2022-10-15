
#pragma once

#include "engine.hpp"

// UI

enum class BorderType {
	SHARP,
	ROUNDED
};

struct ControlArgs {
	Colour32 Foreground;
	Colour32 Background;
	Colour32 Colour;
	BorderType Border;
	Mat4 Rect;
};

// Control

class Control {
protected:
	IDWriteTextLayout* TextLayout;
	DWRITE_TEXT_METRICS TextMetrics;
	Colour32 Foreground;
	Colour32 Background;
	BorderType Border;
	Mat4 Rect;

	Control();
public:
	Control(ControlArgs args);
	virtual ~Control();

	void Draw(Graphics* graphics);
	void SetText(Graphics* graphics, std::wstring text, IDWriteTextFormat* format);
	bool HoveringOver(Mat2 coords);
};

// Panel

class Panel {
private:
	Colour32 Colour;
	BorderType Border;
	Mat4 Rect;
public:
	Panel();
	Panel(ControlArgs args);

	void Draw(Graphics* graphics);
};

// Label

class Label : public Control {
public:
	Label();
	Label(ControlArgs args);
};

// Button

class Button : public Control {
public:
	Button();
	Button(ControlArgs args);
};
