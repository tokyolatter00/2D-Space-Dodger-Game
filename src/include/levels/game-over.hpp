
#pragma once

#include "../level.hpp"
#include "../input.hpp"

class GameOver : public Level {
private:
	InputHandler* Input;

	Panel* Border;
	Label* GameOverLabel;
	Label* ScoreOutputLabel;
	Button* TryAgainButton;
	Button* ExitButton;

	UINT FontLight;
	UINT FontTitle;
public:
	UINT PlayerScore;

	GameOver();

	bool Load() override;
	void Unload() override;
	void Update(float timeTotal, float timeDelta);
	void Draw() override;

	void ProcessKey(UINT uMsg, WPARAM key) override;
	void ProcessMouse(UINT button) override;
};
