#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define MUNCHIECOUNT 50
#define CHERRYCOUNT 25
#define GHOSTCOUNT 5

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

struct Player
{
	// Data to represent Pacman
	bool dead;
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;

	// Sets Pacmans permanent speed
	float _pacmanSpeed;

	// Used for the sprint function
	float speedMultiplier;

	// Using for determining Pacmans direction
	int _pacmanDirection;

	// Used for Pacmans per-frame animation, animation speed and integer for its current frame
	int _pacmanFrameCount;
	int _pacmanFrameTime;
	int _pacmanCurrentFrameTime;

	//Keep score
	int _score;
};

struct EnemyGhost
{
	Vector2* _ghostPosition;
	Texture2D* _ghostTexture;
	Rect* _ghostSourceRect;
	int _ghostDirection;
	float _ghostSpeed;
	int _ghostFrameCount;
	int _ghostFrameTime;
	int _ghostCurrentFrameTime;
};

struct FoodMunchie
{
	// Data to represent Munchie
	Vector2* _munchiePosition;
	Rect* _munchieSourceRect;
	Texture2D* _munchieTexture;

	// Used for the Munchies per-frame animation, animation speed and integer for its current frame
	int _munchieFrameCount;
	int _munchieFrameTime;
	int _munchieCurrentFrameTime;
};

struct FoodCherry
{
	// Data to respresent Cherry
	Vector2* _cherryPosition;
	Rect* _cherrySourceRect;
	Texture2D* _cherryTexture;

	// Used for the Cherries per-frame animation, animation speed and integer for its current frame
	int _cherryFrameCount;
	int _cherryFrameTime;
	int _cherryCurrentFrameTime;
};

struct Paused
{
	Texture2D* _floor;

	// Data for Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	Vector2* _scoreStringPosition;
	bool _paused;

	// Position for String
	Vector2* _stringPosition;

	// Using to test when the start menu is visable
	bool _startMenuPaused;

	// Using for dead screen
	bool _deadScreen;

	// Used to stop screen flickering
	bool _pKeyDown = false;
};

//struct Music
//{
//	bool _musicCheck = true;
//};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	Player* _pacman;
	FoodMunchie* _munchies[MUNCHIECOUNT]; // FoodMunchie* _munchies;
	FoodCherry* _cherries[CHERRYCOUNT];
	EnemyGhost* _ghosts[GHOSTCOUNT];
	Paused* _menu;
	//Music* _music;

	SoundEffect* _pop;
	SoundEffect* _death;
	SoundEffect* _backgroundFunk;
	SoundEffect* _backgroundFunkMuffled;
	SoundEffect* _bingBong;
	
	void CheckMunchieCollisions(FoodMunchie* munchie, int elapsedTime);

	// Controls currentTimeFrames to increase by elapsed time
	void TimeFrameIncrease(int elapsedtime);

	/*void MovementSwap();
	char oldMovementKey;
	char tempKey;
	char newMovementKey;*/

	// Controls Pacmans movement based upon keyboard input
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	// Checks if the game is paused
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);

	// Checks if Pacmanis dead
	void CheckDead(Input::KeyboardState* state, Input::Keys enterKey);

	// Checks if the game has started
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);

	// Checks if Pacman is sprinting
	void CheckSprint(Input::KeyboardState* state, Input::Keys shiftKey);

	// Checks to see if the P key is pressed down to stop flickering when pausing
	void FlickerCheck(Input::KeyboardState* state, Input::Keys pauseKey);

	// Checks what music track to be playing
	//void MusicCheck(Input::KeyboardState* state, Input::Keys pauseKey);

	// Changes textures depending on the frame time
	void FrameTimeCheck();

	// Changes Pacman textures depending on the direction he's facing
	void PacmanDirectionCheck();

	// Updates Pacman, Munchie and Cherry frameCount regards to elapsed time
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(FoodMunchie* munchie, int elapsedTime);
	void UpdateCherry(FoodCherry* cherry, int elapsedTime);
	void UpdateGhost(EnemyGhost* ghost, int elapsedTime);
	void UpdateGhostMovement(EnemyGhost* ghost, int elapsedTime);

	// Check to see if Pacman has crossed the screen border
	void CheckViewportCollision();

	// Checks to see if any of the Ghosts have collided with anything
	void CheckGhostCollisions(EnemyGhost* ghost, int elapsedTime);

	// Used to have a ghost hunt Pacman
	void Hunting(EnemyGhost* ghost, int elapsedTime);

	// Used to have a ghost patrol the border
	void EdgeWalker(EnemyGhost* ghost, int elapsedTime);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};