
#pragma once

#include <iostream>
#include <wincodec.h>
#include "engine.hpp"
#include "types.hpp"

/*
	ANIMATION_TYPE_STATIC		- Render a still image
	ANIMATION_TYPE_REPEAT		- Render a sequence of images on repeat
*/

enum class AnimationType {
	STATIC,
	REPEAT
};

class SpriteSheet {
private:
	Graphics* Gfx;
	ID2D1Bitmap* Bitmap;
	AnimationType Animation;

	UINT Width;
	UINT Height;
	UINT SpriteCount;
	UINT Frame;

	void DrawAnimationStatic(Mat2 location);
	void DrawAnimationRepeat(Mat2 location);
public:
	SpriteSheet(std::string filename, Graphics* graphics, AnimationType animation);
	SpriteSheet(std::string filename, Graphics* graphics, AnimationType animation, int width, int height);
	~SpriteSheet();

	UINT GetWidth();
	UINT GetHeight();
	UINT GetSpriteCount();
	Mat2 GetSize();

	void Draw(Mat2 location);
};
