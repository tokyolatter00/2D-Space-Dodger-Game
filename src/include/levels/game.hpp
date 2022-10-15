
#pragma once

/*
		wchar_t text_buffer[200] = { 0 }; //temporary buffer
		swprintf(text_buffer, _countof(text_buffer), L"%d\n", i); // convert
		OutputDebugString(text_buffer); // print
*/

/*
	Screen Block Width: 50
	Screen Block Height: 40
*/

#include "../level.hpp"
#include "../input.hpp"
#include "../random.hpp"

class SpaceShip;
class SpikeBall;
class Coin;
class SideObject;

// Game

class Game: public Level {
private:
	InputHandler* Input;
	RandomGenerator* Random;
	SpaceShip* Player;

	std::vector<SpikeBall>* Enemies;
	std::vector<Coin>* Coins;
	std::vector<SideObject>* SideObjects;
	Stopwatch* EntityTimer;
	Stopwatch* SideObjectTimer;

	Label* GameOutputLabel;
	Panel* Grass;
	Panel* Dividers;
	UINT FontLight;

	void SpawnEnemy();
	void SpawnCoin();
	void SpawnSideObject();
	void DespawnEnemy(UINT id);
	void DespawnCoin(UINT id);
	void DespawnSideObject(UINT id);
public:
	bool Load() override;
	void Unload() override;
	void Update(float timeTotal, float timeDelta) override;
	void Draw() override;

	void ProcessKey(UINT uMsg, WPARAM key) override;
	void ProcessMouse(UINT button) override;
};

// Sprites

class Sprites {
public:
	static SpriteSheet* SpaceShip;
	static SpriteSheet* SpikeBall;
	static SpriteSheet* Coin;
	static SpriteSheet* Tree;
	static SpriteSheet* Rock;

	static void Load(Graphics* graphics);
	static void Unload();
};

// Object

class Object {
protected:
	SpriteSheet* Sprite;
	Mat2 Location;
public:
	Object(Mat2 location);

	float GetX();
	float GetY();
	float GetLeft();
	float GetRight();
	float GetTop();
	float GetBottom();
};

// Entity

class Entity : public Object {
protected:
	UINT Lane;
public:
	Entity(Mat2 location, UINT lane);

	void Update(float timeTotal, float timeDelta);
	void Draw();

	UINT GetLane();
	bool CollidedWith(Entity* entity);
};

// Space Ship

class SpaceShip : public Entity {
private:
	UINT Health;
	UINT Score;
public:
	SpaceShip(UINT lane);
	
	void Update(float timeTotal, float timeDelta);
	void Draw();

	void Damage();
	void AddPoint();

	UINT GetHealth();
	UINT GetScore();

	void MoveLeft();
	void MoveRight();
};

// Spike Ball

class SpikeBall : public Entity {
public:
	SpikeBall(Mat2 location, UINT lane);

	void Update(float timeTotal, float timeDelta);
	void Draw();
};

// Coin

class Coin : public Entity {
public:
	Coin(Mat2 location, UINT lane);

	void Update(float timeTotal, float timeDelta);
	void Draw();
};

// Side Object

enum class SideObjectType {
	Tree,
	Rock
};

class SideObject : public Object {
private:
	SideObjectType Type;
	SpriteSheet* Sprite;
public:
	SideObject(SideObjectType type, Mat2 location);

	void Update(float timeTotal, float timeDelta);
	void Draw();
};
