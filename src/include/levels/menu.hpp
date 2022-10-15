
#pragma once

#include "../level.hpp"
#include "../input.hpp"

class Menu : public Level {
private:
	InputHandler* Input;

	Panel* Border;
	Label* TitleLabel;
	Button* StartButton;

	UINT FontLight;
	UINT FontTitle;
public:
	bool Load() override;
	void Unload() override;
	void Update(float timeTotal, float timeDelta) override;
	void Draw() override;

	void ProcessKey(UINT uMsg, WPARAM key) override;
	void ProcessMouse(UINT button) override;
};
