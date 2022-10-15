
#include "include/engine.hpp"

// Colour32

Colour32::Colour32() {
	R = 0.0f;
	G = 0.0f;
	B = 0.0f;
	A = 0.0f;
}

Colour32::Colour32(float r, float g, float b, float a) {
	R = r;
	G = g;
	B = b;
	A = a;
}

Colour32::Colour32(float r, float g, float b) {
	R = r;
	G = g;
	B = b;
	A = 1.0f;
}

D2D1::ColorF Colour32::ToColourF() {
	return D2D1::ColorF(R, G, B, A);
}

// Mat2

Mat2::Mat2() {
	M1 = 0;
	M2 = 0;
}

Mat2::Mat2(float x, float y) {
	M1 = x;
	M2 = y;
}

float Mat2::GetX() {
	return M1;
}

float Mat2::GetY() {
	return M2;
}

float Mat2::GetWidth() {
	return M1;
}

float Mat2::GetHeight() {
	return M2;
}

void Mat2::SetX(float x) {
	M1 = x;
}

void Mat2::SetY(float y) {
	M2 = y;
}

void Mat2::SetWidth(float width) {
	M1 = width;
}

void Mat2::SetHeight(float height) {
	M2 = height;
}

D2D1_POINT_2F Mat2::ToPointF() {
	return D2D1::Point2F(M1, M2);
}

// Mat4

Mat4::Mat4() {
	M1 = 0;
	M2 = 0;
	M3 = 0;
	M4 = 0;
}

Mat4::Mat4(float m1, float m2, float m3, float m4) {
	M1 = m1;
	M2 = m2;
	M3 = m3;
	M4 = m4;
}

float Mat4::GetX() {
	return M1;
}

float Mat4::GetY() {
	return M2;
}

float Mat4::GetWidth() {
	return M3;
}

float Mat4::GetHeight() {
	return M4;
}

void Mat4::SetX(float x) {
	M1 = x;
}

void Mat4::SetY(float y) {
	M2 = y;
}

void Mat4::SetWidth(float width) {
	M3 = width;
}

void Mat4::SetHeight(float height) {
	M4 = height;
}

D2D1_RECT_F Mat4::ToRectF() {
	return D2D1::RectF(M1, M2, M3, M4);
}

// Graphics

Graphics::Graphics() {
	Factory = NULL;
	RenderTarget = NULL;
	SolidBrush = NULL;
	Formats = new std::vector<IDWriteTextFormat*>();
}

Graphics::~Graphics() {
	if (Factory) {
		Factory->Release();
	}

	if (RenderTarget) {
		RenderTarget->Release();
	}

	if (SolidBrush) {
		SolidBrush->Release();
	}

	// Delete Formats

	for (ULLONG i = 0; i < Formats->size(); i++) {
		Formats->at(i)->Release();
	}

	delete Formats;
}

bool Graphics::Init(HWND hWnd) {
	HWnd = hWnd;

	HRESULT res;

	// Create Factory

	res = D2D1CreateFactory(
		D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&Factory
	);

	if (FAILED(res)) {
		return false;
	}

	// Create Render Target

	RECT rect;
	GetClientRect(hWnd, &rect);

	D2D1_HWND_RENDER_TARGET_PROPERTIES hWnd_props;
	hWnd_props.hwnd = hWnd;
	hWnd_props.presentOptions = D2D1_PRESENT_OPTIONS_NONE; //D2D1_PRESENT_OPTIONS_IMMEDIATELY;
	hWnd_props.pixelSize = D2D1::SizeU(rect.right, rect.bottom);

	res = Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		hWnd_props,
		&RenderTarget
	);

	if (FAILED(res)) {
		return false;
	}

	// Create Write Factory

	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&WriteFactory
	);

	if (FAILED(res)) {
		return false;
	}

	// Create Brush

	Colour32 brushColour = Colour32(1.0f, 0.0f, 0.0f);
	res = RenderTarget->CreateSolidColorBrush(brushColour.ToColourF(), &SolidBrush);

	if (FAILED(res)) {
		return false;
	}

	return true;
}

HWND Graphics::GetHWnd() {
	return HWnd;
}

ID2D1Factory* Graphics::GetFactory() {
	return Factory;
}

ID2D1RenderTarget* Graphics::GetRenderTarget() {
	return RenderTarget;
}

IDWriteFactory* Graphics::GetWriteFactory() {
	return WriteFactory;
}

void Graphics::BeginDraw() {
	RenderTarget->BeginDraw();
}

void Graphics::EndDraw() {
	RenderTarget->EndDraw();
}

void Graphics::ClearScreen(Colour32 colour) {
	RenderTarget->Clear(colour.ToColourF());
}

UINT Graphics::CreateTextFormat(std::wstring font_name, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, float size) {
	// Create Text Format

	IDWriteTextFormat* format;
	HRESULT res;

	res = WriteFactory->CreateTextFormat(
		font_name.c_str(),
		NULL,
		weight,
		style,
		stretch,
		size,
		L"en-us",
		&format
	);

	// Use Centered Text Alignment
	
	format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	// Add to active text formats

	Formats->push_back(format);
	return Formats->size() - 1;
}

IDWriteTextLayout* Graphics::CreateTextLayout(std::wstring str, IDWriteTextFormat* format, float max_width, float max_height) {
	// Create Text Layout

	IDWriteTextLayout* layout;
	HRESULT res;

	res = WriteFactory->CreateTextLayout(
		str.c_str(),
		str.length(),
		format,
		max_width,
		max_height,
		&layout
	);

	return layout;
}

void Graphics::ClearTextFormats() {
	// Clear active text formats

	for (ULLONG i = 0; i < Formats->size(); i++) {
		Formats->at(i)->Release();
	}

	Formats->clear();
}

IDWriteTextFormat* Graphics::GetTextFormat(UINT id) {
	// Return text format at position at id (will fail if id is out of bounds)

	return Formats->at(id);
}

void Graphics::DrawLine(Mat2 point0, Mat2 point1, Colour32 colour, float size) {
	SolidBrush->SetColor(colour.ToColourF());

	RenderTarget->DrawLine(
		point0.ToPointF(),
		point1.ToPointF(),
		SolidBrush,
		size
	);
}

void Graphics::DrawCircle(Mat2 location, float radius, Colour32 colour, bool fill) {
	SolidBrush->SetColor(colour.ToColourF());

	if (fill) {
		RenderTarget->FillEllipse(
			D2D1::Ellipse(
				location.ToPointF(),
				radius, radius
			),
			SolidBrush
		);
	}
	else {
		RenderTarget->DrawEllipse(
			D2D1::Ellipse(
				location.ToPointF(),
				radius, radius
			),
			SolidBrush,
			3.0f
		);
	}
}

void Graphics::DrawRectangle(Mat4 rect, Colour32 colour, bool fill) {
	SolidBrush->SetColor(colour.ToColourF());

	if (fill) {
		RenderTarget->FillRectangle(
			Mat4(
				rect.GetX(),
				rect.GetY(),
				rect.GetWidth() + rect.GetX(),
				rect.GetHeight() + rect.GetY()
			).ToRectF(),
			SolidBrush
		);
	}
	else {
		RenderTarget->DrawRectangle(
			Mat4(
				rect.GetX(),
				rect.GetY(),
				rect.GetWidth() + rect.GetX(),
				rect.GetHeight() + rect.GetY()
			).ToRectF(),
			SolidBrush
		);
	}
}

void Graphics::DrawRoundedRectangle(Mat4 rect, Colour32 colour, float round, bool fill) {
	SolidBrush->SetColor(colour.ToColourF());

	if (fill) {
		RenderTarget->FillRoundedRectangle(
			D2D1::RoundedRect(
				Mat4(
					rect.GetX(),
					rect.GetY(),
					rect.GetWidth() + rect.GetX(),
					rect.GetHeight() + rect.GetY()
				).ToRectF(),
				round,
				round
			),
			SolidBrush
		);
	}
	else {
		RenderTarget->DrawRoundedRectangle(
			D2D1::RoundedRect(
				Mat4(
					rect.GetX(),
					rect.GetY(),
					rect.GetWidth() + rect.GetX(),
					rect.GetHeight() + rect.GetY()
				).ToRectF(),
				round,
				round
			),
			SolidBrush
		);
	}
}

void Graphics::DrawText(IDWriteTextLayout* layout, Mat2 location, Colour32 colour) {
	SolidBrush->SetColor(colour.ToColourF());

	RenderTarget->DrawTextLayout(
		location.ToPointF(),
		layout,
		SolidBrush
	);
}
