
#pragma once

#include <iostream>
#include <vector>
#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "types.hpp"

#undef DrawText

#define Colour32Transparent					Colour32(0.0f, 0.0f, 0.0f, 0.0f)
#define Colour32White						Colour32(1.0f, 1.0f, 1.0f)
#define Colour32Black						Colour32(0.0f, 0.0f, 0.0f)
#define Colour32DarkGray					Colour32(0.1176f, 0.1294f, 0.1411f)
#define Colour32Gray						Colour32(0.2117f, 0.2235f, 0.2431f)
#define Colour32LightGray					Colour32(0.2588f, 0.2705f, 0.2862f)
#define Colour32GrassGreen					Colour32(0.4196f, 0.6117f, 0.3450f)

/*
	Colour32 - 32 Bit RGBA Colour
*/

class Colour32 {
public:
	float R;
	float G;
	float B;
	float A;

	Colour32();
	Colour32(float r, float g, float b, float a);
	Colour32(float r, float g, float b);

	D2D1::ColorF ToColourF();
};

/*
	Mat2 - 2 Element Matrix
*/

class Mat2{
private:
	float M1;
	float M2;
public:
	Mat2();
	Mat2(float m1, float m2);

	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	void SetX(float x);
	void SetY(float y);
	void SetWidth(float width);
	void SetHeight(float height);

	D2D1_POINT_2F ToPointF();
};

/*
	Mat4 - 4 Element Matrix
*/

class Mat4 {
private:
	float M1;
	float M2;
	float M3;
	float M4;
public:
	Mat4();
	Mat4(float m1, float m2, float m3, float m4);

	float GetX();
	float GetY();
	float GetWidth();
	float GetHeight();

	void SetX(float x);
	void SetY(float y);
	void SetWidth(float width);
	void SetHeight(float height);

	D2D1_RECT_F ToRectF();
};

/*
	Graphics - Graphical Rendering Implementations
*/

class Graphics {
private:
	HWND HWnd;
	ID2D1Factory* Factory;
	ID2D1HwndRenderTarget* RenderTarget;
	IDWriteFactory* WriteFactory;
	ID2D1SolidColorBrush* SolidBrush;

	std::vector<IDWriteTextFormat*>* Formats;
public:
	Graphics();
	~Graphics();
	bool Init(HWND hWnd);

	HWND GetHWnd();
	ID2D1Factory* GetFactory();
	ID2D1RenderTarget* GetRenderTarget();
	IDWriteFactory* GetWriteFactory();

	void BeginDraw();
	void EndDraw();
	void ClearScreen(Colour32 colour);

	UINT CreateTextFormat(std::wstring font_name, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch, float size);
	IDWriteTextLayout* CreateTextLayout(std::wstring str, IDWriteTextFormat* format, float max_width, float max_height);
	void ClearTextFormats();
	IDWriteTextFormat* GetTextFormat(UINT id);

	void DrawLine(Mat2 point0, Mat2 point1, Colour32 colour, float size);
	void DrawCircle(Mat2 location, float radius, Colour32 colour, bool fill = true);
	void DrawRectangle(Mat4 rect, Colour32 colour, bool fill = true);
	void DrawRoundedRectangle(Mat4 rect, Colour32 colour, float round, bool fill = true);
	void DrawText(IDWriteTextLayout* layout, Mat2 location, Colour32 colour);
};
