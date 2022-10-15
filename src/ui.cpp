
#include "include/ui.hpp"

// Control

Control::Control() {

}

Control::Control(ControlArgs args) {
	TextLayout = nullptr;
	Foreground = args.Foreground;
	Background = args.Background;
	Border = args.Border;
	Rect = args.Rect;
}

Control::~Control() {
	if (TextLayout) {
		TextLayout->Release();
	}
}

void Control::Draw(Graphics* graphics) {
	// Draw Border Rectangle

	switch (Border) {
		case BorderType::SHARP:
			graphics->DrawRectangle(
				Rect,
				Background,
				25.0f
			);
			break;
		case BorderType::ROUNDED:
			graphics->DrawRoundedRectangle(
				Rect,
				Background,
				25.0f
			);
			break;
	}

	// Draw Foreground Text

	graphics->DrawText(
		TextLayout,
		Mat2(
			Rect.GetX(),
			Rect.GetY() + ((Rect.GetHeight() - TextMetrics.height) / 2)
		),
		Foreground
	);
}

void Control::SetText(Graphics* graphics, std::wstring text, IDWriteTextFormat* format) {
	// Delete Previous Text Layout

	if (TextLayout) {
		TextLayout->Release();
	}

	// Create New Text Layout

	TextLayout = graphics->CreateTextLayout(
		text,
		format,
		Rect.GetWidth(),
		Rect.GetHeight()
	);

	// Get Text Metrics

	TextLayout->GetMetrics(&TextMetrics);
}

bool Control::HoveringOver(Mat2 coords) {
	// Return true if coords is inside of the control bounds

	return coords.GetX() > Rect.GetX()
		&& coords.GetX() < (Rect.GetX() + Rect.GetWidth())
		&& coords.GetY() > Rect.GetY()
		&& coords.GetY() < (Rect.GetY() + Rect.GetHeight());
}

// Panel

Panel::Panel() {

}

Panel::Panel(ControlArgs args) {
	Colour = args.Colour;
	Border = args.Border;
	Rect = args.Rect;
}

void Panel::Draw(Graphics* graphics) {
	// Draw Border Rectangle

	switch (Border) {
		case BorderType::SHARP:
			graphics->DrawRectangle(
				Rect,
				Colour
			);
			break;
		case BorderType::ROUNDED:
			graphics->DrawRoundedRectangle(
				Rect,
				Colour,
				25.0f
			);
			break;
	}
}

// Label

Label::Label() {

}

Label::Label(ControlArgs args) : Control(args) {

}

// Button

Button::Button() {

}

Button::Button(ControlArgs args) : Control(args) {

}
