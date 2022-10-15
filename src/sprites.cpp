
#include "include/sprites.hpp"

SpriteSheet::SpriteSheet(std::string filename, Graphics* graphics, AnimationType animation) {
	Bitmap = nullptr;
	Frame = 0;

	Gfx = graphics;
	Animation = animation;

	HRESULT res;

	// Create WIC Factory

	IWICImagingFactory* factory = nullptr;

	res = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&factory
	);

	// Load Image from File

	IWICBitmapDecoder* decoder = nullptr;
	std::wstring wfilename = std::wstring(filename.begin(), filename.end());

	res = factory->CreateDecoderFromFilename(
		wfilename.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&decoder
	);

	// Read WIC Bitmap from Image

	IWICBitmapFrameDecode* frame = nullptr;

	res = decoder->GetFrame(
		0,
		&frame
	);

	// Create Flipped WIC Bitmap

	IWICBitmapFlipRotator* flipped = nullptr;

	res = factory->CreateBitmapFlipRotator(
		&flipped
	);

	res = flipped->Initialize(
		frame,
		WICBitmapTransformFlipHorizontal
	);

	// Create D2D1 Bitmap
	
	IWICFormatConverter* converter = nullptr;

	res = factory->CreateFormatConverter(
		&converter
	);

	res = converter->Initialize(
		frame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0,
		WICBitmapPaletteTypeCustom
	);

	res = Gfx->GetRenderTarget()->CreateBitmapFromWicBitmap(
		converter,
		NULL,
		&Bitmap
	);

	// Initialise Properties

	Width = Bitmap->GetSize().width;
	Height = Bitmap->GetSize().height;
	SpriteCount = 1;

	// Memory Cleanup

	if (factory) {
		factory->Release();
	}

	if (decoder) {
		decoder->Release();
	}

	if (frame) {
		frame->Release();
	}

	if (converter) {
		converter->Release();
	}
}

SpriteSheet::SpriteSheet(std::string filename, Graphics* graphics, AnimationType animation, int width, int height)
	: SpriteSheet(filename, graphics, animation) 
{
	// Used to create sprite for a singular static image

	Width = width;
	Height = height;
	SpriteCount = Bitmap->GetSize().width / Width;
}

SpriteSheet::~SpriteSheet() {
	if (Bitmap) {
		Bitmap->Release();
	}
}

UINT SpriteSheet::GetWidth() {
	return Width;
}

UINT SpriteSheet::GetHeight() {
	return Height;
}

UINT SpriteSheet::GetSpriteCount() {
	return SpriteCount;
}

Mat2 SpriteSheet::GetSize() {
	return Mat2(Width, Height);
}

void SpriteSheet::DrawAnimationStatic(Mat2 location) {
	// Source rect (draw entire bitmap image)

	Mat4 src_rect = Mat4(
		0.0f,
		0.0f,
		Bitmap->GetSize().width,
		Bitmap->GetSize().height
	);

	// Dest rect (draw using location coordinates)

	Mat4 dest_rect = Mat4(
		location.GetX(),
		location.GetY(),
		location.GetX() + Bitmap->GetSize().width,
		location.GetY() + Bitmap->GetSize().height
	);

	// Draw Bitmap to Render Target

	Gfx->GetRenderTarget()->DrawBitmap(
		Bitmap,
		dest_rect.ToRectF(),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		src_rect.ToRectF()
	);
}

void SpriteSheet::DrawAnimationRepeat(Mat2 location) {
	// Get current frame in the animation

	UINT index = (Frame / 10) % SpriteCount;
	float src_x = (index % SpriteCount) * Width;
	float src_y = (index / SpriteCount) * Height;

	// Source rect (draw entire bitmap image)

	Mat4 src_rect = Mat4(
		src_x,
		src_y,
		src_x + Width,
		src_y + Height
	);

	// Dest rect (draw using location coordinates)

	Mat4 dest_rect = Mat4(
		location.GetX(),
		location.GetY(),
		location.GetX() + Width,
		location.GetY() + Height
	);

	// Draw Bitmap

	Gfx->GetRenderTarget()->DrawBitmap(
		Bitmap,
		dest_rect.ToRectF(),
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		src_rect.ToRectF()
	);

	Frame++;
}

void SpriteSheet::Draw(Mat2 location) {
	switch (Animation) {
		case AnimationType::STATIC:
			// Draw static bitmap

			DrawAnimationStatic(location);
			break;
		case AnimationType::REPEAT:
			// Draw sprite sheet animation on repeat

			DrawAnimationRepeat(location);
			break;
	}
}
