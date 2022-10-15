
#include <sstream>
#include "include/controller.hpp"
#include "include/levels/game.hpp"

// Window

#define WINDOW_WIDTH						640
#define WINDOW_HEIGHT						480

// Lanes

#define LANE_ONE							1
#define LANE_TWO							2
#define LANE_THREE							3
#define LANE_ONE_X							170.0f
#define LANE_TWO_X							170.0f + 100.0f
#define LANE_THREE_X						170.0f + 200.0f
#define LANE_OFFSET							(100.0f - 64.0f) / 2.0f

// Grass

#define GRASS_WIDTH							170
#define GRASS_HEIGHT						480

// Player / Space Ship

#define SPACESHIP_WIDTH						64
#define SPACESHIP_HEIGHT					64
#define PLAYER_POS_Y						480.0f - 200.0f
#define PLAYER_START_HEALTH					5
#define PLAYER_START_SCORE					0

// Enemy / Spike Ball

#define SPIKEBALL_WIDTH						64
#define SPIKEBALL_HEIGHT					58

// Coin

#define COIN_WIDTH							64
#define COIN_HEIGHT							64

// Side Object

#define SIDE_OBJECT_WIDTH					32
#define SIDE_OBJECT_HEIGHT					32

// Game

bool Game::Load() {
	Sprites::Load(Gfx);

	// Create Objects

	Input = new InputHandler();
	Random = new RandomGenerator();
	Player = new SpaceShip(LANE_TWO);

	Enemies = new std::vector<SpikeBall>();
	Coins = new std::vector<Coin>();
	SideObjects = new std::vector<SideObject>();
	EntityTimer = new Stopwatch();
	SideObjectTimer = new Stopwatch();

	// Create Fonts

	FontLight = Gfx->CreateTextFormat(
		L"Lucida Console",
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15.0f
	);

	// Load UI

	ControlArgs args;

	Grass = new Panel[2];
	Dividers = new Panel[2];

	// Load UI - Game Output Label

	args.Foreground = Colour32White;
	args.Background = Colour32DarkGray;
	args.Border = BorderType::ROUNDED;
	args.Rect = Mat4(25.0f, 25.0f, 125.0f, 100.0f);

	GameOutputLabel = new Label(args);

	// Load UI - Grass 1

	args.Colour = Colour32GrassGreen;
	args.Border = BorderType::SHARP;
	args.Rect = Mat4(0.0f, 0.0f, GRASS_WIDTH, WINDOW_HEIGHT);

	Grass[0] = Panel(args);

	// Load UI - Grass 2

	args.Colour = Colour32GrassGreen;
	args.Border = BorderType::SHARP;
	args.Rect = Mat4(WINDOW_WIDTH - GRASS_WIDTH, 0.0f, GRASS_WIDTH, WINDOW_HEIGHT);

	Grass[1] = Panel(args);

	// Load UI - Divider 1

	args.Colour = Colour32White;
	args.Border = BorderType::SHARP;
	args.Rect = Mat4(170.0f + 100.0f, 0.0f, 5.0f, WINDOW_HEIGHT);

	Dividers[0] = Panel(args);

	// Load UI = Divider 2

	args.Colour = Colour32White;
	args.Border = BorderType::SHARP;
	args.Rect = Mat4(170.0f + 200.0f, 0.0f, 5.0f, WINDOW_HEIGHT);

	Dividers[1] = Panel(args);

	return true;
}

void Game::Unload() {
	Sprites::Unload();
	Gfx->ClearTextFormats();

	delete Input;
	delete Random;
	delete Player;
	delete Enemies;
	delete Coins;
	delete SideObjects;
	delete EntityTimer;
	delete SideObjectTimer;
	delete[] Grass;
	delete[] Dividers;
	delete GameOutputLabel;
}

void Game::Update(float timeTotal, float timeDelta) {
	// Update Player

	Player->Update(timeTotal, timeDelta);

	// Update Enemies

	for (ULLONG i = 0; i < Enemies->size(); i++) {
		SpikeBall* enemy = &Enemies->at(i);
		enemy->Update(timeTotal, timeDelta);

		// Check if enemy should be despawned

		if (enemy->GetTop() > WINDOW_HEIGHT) {
			DespawnEnemy(i);
			continue;
		}

		// Check for collision with the player

		if (Player->CollidedWith(enemy)) {
			Player->Damage();
			DespawnEnemy(i);
		}
	}

	// Update Coins

	for (ULLONG i = 0; i < Coins->size(); i++) {
		Coin* coin = &Coins->at(i);
		coin->Update(timeTotal, timeDelta);

		// Check if coin should be despawned

		if (coin->GetTop() > WINDOW_HEIGHT) {
			DespawnCoin(i);
			continue;
		}

		// Check for collision with the player

		if (Player->CollidedWith(coin)) {
			Player->AddPoint();
			DespawnCoin(i);
		}
	}

	// Update Side Objects

	for (ULLONG i = 0; i < SideObjects->size(); i++) {
		SideObject* object = &SideObjects->at(i);
		object->Update(timeTotal, timeDelta);

		// Check if side object should be despawned

		if (object->GetTop() > WINDOW_HEIGHT) {
			DespawnSideObject(i);
			continue;
		}
	}

	// Spawn New Entity
	
	if (EntityTimer->GetElapsed() > MILLISECONDS(500)) {
		if (Random->ConFlip()) {
			SpawnEnemy();
		}
		else {
			SpawnCoin();
		}

		EntityTimer->Restart();
	}
	
	// Spawn New Side Object

	if (SideObjectTimer->GetElapsed() > MILLISECONDS(50)) {
		SpawnSideObject();

		SideObjectTimer->Restart();
	}

	// Detect 'A' Key

	if (Input->KeyIsPressed(KEY_A)) {
		Player->MoveLeft();
	}

	// Detect 'D' Key

	if (Input->KeyIsPressed(KEY_D)) {
		Player->MoveRight();
	}
	
	// Check if player is still alive

	if (Player->GetHealth() == 0) {
		*Result = new LevelResult(LevelExitCode::START_GAME_OVER, Player->GetScore());
		return;
	}

	// Update Title

	std::wstringstream wss;
	wss << "Direct 2D Window";
	wss << " | ";
	wss << std::to_wstring(*FPS);
	wss << " FPS";

	SetWindowText(HWnd, wss.str().c_str());
}

void Game::Draw() {
	// Draw UI

	Grass[0].Draw(Gfx);
	Grass[1].Draw(Gfx);
	Dividers[0].Draw(Gfx);
	Dividers[1].Draw(Gfx);

	// Draw Player

	Player->Draw();

	// Draw Enemies

	for (ULLONG i = 0; i < Enemies->size(); i++) {
		SpikeBall* enemy = &Enemies->at(i);
		enemy->Draw();
	}

	// Draw Coins

	for (ULLONG i = 0; i < Coins->size(); i++) {
		Coin* coin = &Coins->at(i);
		coin->Draw();
	}

	// Draw Side Objects

	for (ULLONG i = 0; i < SideObjects->size(); i++) {
		SideObject* object = &SideObjects->at(i);
		object->Draw();
	}

	// Draw Text Output

	std::wstringstream wss;
	wss << "Health: ";
	wss << std::to_wstring(Player->GetHealth());
	wss << '\n';
	wss << "Score: ";
	wss << std::to_wstring(Player->GetScore());

	GameOutputLabel->SetText(
		Gfx,
		wss.str(),
		Gfx->GetTextFormat(FontLight)
	);

	GameOutputLabel->Draw(Gfx);
}

void Game::ProcessKey(UINT uMsg, WPARAM key) {
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

void Game::ProcessMouse(UINT button) {
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

void Game::SpawnEnemy() {
	Mat2 location = Mat2(
		0.0f,
		-SPIKEBALL_HEIGHT
	);

	UINT lane = Random->RandInt(
		LANE_ONE,
		LANE_THREE
	);

	SpikeBall enemy = SpikeBall(
		location,
		lane
	);

	Enemies->push_back(enemy);
}

void Game::SpawnCoin() {
	Mat2 location = Mat2(
		0.0f,
		-COIN_HEIGHT
	);

	UINT lane = Random->RandInt(
		LANE_ONE,
		LANE_THREE
	);

	Coin coin = Coin(
		location,
		lane
	);

	Coins->push_back(coin);
}

void Game::SpawnSideObject() {
	SideObjectType type = (SideObjectType)Random->RandInt(0, 1);
	Mat2 location;

	if (Random->ConFlip()) {
		location = Mat2(
			Random->RandInt(0.0f, 170.0f - SIDE_OBJECT_WIDTH),
			-SIDE_OBJECT_HEIGHT
		);
	}
	else {
		location = Mat2(
			Random->RandInt(170.0f + 300.0f, (170.0f + 300.0f + GRASS_WIDTH) - SIDE_OBJECT_WIDTH),
			-SIDE_OBJECT_HEIGHT
		);
	}

	SideObject object = SideObject(
		type,
		location
	);

	SideObjects->push_back(object);
}

void Game::DespawnEnemy(UINT id) {
	Enemies->erase(Enemies->begin() + id);
}

void Game::DespawnCoin(UINT id) {
	Coins->erase(Coins->begin() + id);
}

void Game::DespawnSideObject(UINT id) {
	SideObjects->erase(SideObjects->begin() + id);
}

// Sprites

SpriteSheet* Sprites::SpaceShip;
SpriteSheet* Sprites::SpikeBall;
SpriteSheet* Sprites::Coin;
SpriteSheet* Sprites::Tree;
SpriteSheet* Sprites::Rock;

void Sprites::Load(Graphics* graphics) {
	SpaceShip = new SpriteSheet(
		"resources/sprites/spaceship.png",
		graphics,
		AnimationType::STATIC
	);

	SpikeBall = new SpriteSheet(
		"resources/sprites/spikeball.png",
		graphics,
		AnimationType::STATIC
	);

	Coin = new SpriteSheet(
		"resources/sprites/coin.png",
		graphics,
		AnimationType::REPEAT,
		COIN_WIDTH,
		COIN_HEIGHT
	);

	Tree = new SpriteSheet(
		"resources/sprites/tree.png",
		graphics,
		AnimationType::STATIC
	);

	Rock = new SpriteSheet(
		"resources/sprites/rock.png",
		graphics,
		AnimationType::STATIC
	);
}

void Sprites::Unload() {
	delete SpaceShip;
	delete SpikeBall;
	delete Coin;
	delete Tree;
	delete Rock;
}

// Object

Object::Object(Mat2 location) {
	Sprite = nullptr;
	Location = location;
}

float Object::GetX() {
	return Location.GetX();
}

float Object::GetY() {
	return Location.GetY();
}

float Object::GetLeft() {
	return Location.GetX();
}

float Object::GetRight() {
	return Location.GetX() + Sprite->GetWidth();
}

float Object::GetTop() {
	return Location.GetY();
}

float Object::GetBottom() {
	return Location.GetY() + Sprite->GetHeight();
}

// Entity

Entity::Entity(Mat2 location, UINT lane) : Object(location) {
	Lane = lane;
}

void Entity::Update(float timeTotal, float timeDelta) {
	
}

void Entity::Draw() {
	Sprite->Draw(Location);
}

UINT Entity::GetLane() {
	return Lane;
}

bool Entity::CollidedWith(Entity* entity) {
	// Return true if a collision has been detected between entities

	if (GetLane() != entity->GetLane()) {
		return false;
	}

	if (GetBottom() > entity->GetTop() && GetTop() < entity->GetBottom()) {
		return true;
	}

	return false;
}

// Space Ship

SpaceShip::SpaceShip(UINT lane) : Entity(Mat2(0.0f, PLAYER_POS_Y), lane) {
	Sprite = Sprites::SpaceShip;
	Health = PLAYER_START_HEALTH;
	Score = PLAYER_START_SCORE;
}

void SpaceShip::Update(float timeTotal, float timeDelta) {
	Entity::Update(timeTotal, timeDelta);

	switch (Lane) {
		case LANE_ONE:
			Location.SetX(LANE_ONE_X + LANE_OFFSET);
			break;
		case LANE_TWO:
			Location.SetX(LANE_TWO_X + LANE_OFFSET);
			break;
		case LANE_THREE:
			Location.SetX(LANE_THREE_X + LANE_OFFSET);
			break;
	}
}

void SpaceShip::Draw() {
	Entity::Draw();
}

void SpaceShip::Damage() {
	if (Health == 0) {
		return;
	}

	Health--;
}

void SpaceShip::AddPoint() {
	Score++;
}

UINT SpaceShip::GetHealth() {
	return Health;
}

UINT SpaceShip::GetScore() {
	return Score;
}

void SpaceShip::MoveLeft() {
	if (Lane > LANE_ONE) {
		Lane--;
	}
}

void SpaceShip::MoveRight() {
	if (Lane < LANE_THREE) {
		Lane++;
	}
}

// Spike Ball

SpikeBall::SpikeBall(Mat2 location, UINT lane) : Entity(location, lane) {
	Sprite = Sprites::SpikeBall;

	switch (Lane) {
		case LANE_ONE:
			Location.SetX(LANE_ONE_X + LANE_OFFSET);
			break;
		case LANE_TWO:
			Location.SetX(LANE_TWO_X + LANE_OFFSET);
			break;
		case LANE_THREE:
			Location.SetX(LANE_THREE_X + LANE_OFFSET);
			break;
	}
}

void SpikeBall::Update(float timeTotal, float timeDelta) {
	Entity::Update(timeTotal, timeDelta);

	Location.SetY(Location.GetY() + (550.0f * timeDelta));
}

void SpikeBall::Draw() {
	Entity::Draw();
}

// Coin

Coin::Coin(Mat2 location, UINT lane) : Entity(location, lane) {
	Sprite = Sprites::Coin;

	switch (Lane) {
		case LANE_ONE:
			Location.SetX(LANE_ONE_X + LANE_OFFSET);
			break;
		case LANE_TWO:
			Location.SetX(LANE_TWO_X + LANE_OFFSET);
			break;
		case LANE_THREE:
			Location.SetX(LANE_THREE_X + LANE_OFFSET);
			break;
	}
}

void Coin::Update(float timeTotal, float timeDelta) {
	Entity::Update(timeTotal, timeDelta);
	
	Location.SetY(Location.GetY() + (550.0f * timeDelta));
}

void Coin::Draw() {
	Entity::Draw();
}

// Side Object

SideObject::SideObject(SideObjectType type, Mat2 location) : Object(location) {
	Type = type;

	switch (Type) {
		case SideObjectType::Tree:
			Sprite = Sprites::Tree;
			break;
		case SideObjectType::Rock:
			Sprite = Sprites::Rock;
			break;
	}
}

void SideObject::Update(float timeTotal, float timeDelta) {
	Location.SetY(Location.GetY() + (550.0f * timeDelta));
}

void SideObject::Draw() {
	Sprite->Draw(Location);
}
