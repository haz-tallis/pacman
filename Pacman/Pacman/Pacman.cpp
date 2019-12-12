#include "Pacman.h"
#include <sstream>
#include <time.h>
#include <Windows.h>
#include <iostream>

// Main variables below

// Constructor Method
Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	// Local Variables
	int i;
	srand(time(NULL));

	//_munchies = new FoodMunchie[MUNCHIECOUNT];

	//Initialise Munchies
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new FoodMunchie();
		_munchies[i]->_munchieFrameTime = rand() % 500 + 50;
		_munchies[i]->_munchieFrameCount = rand() % 1;
		_munchies[i]->_munchieCurrentFrameTime = 0;
	}

	//Initialise Cherries
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i] = new FoodCherry();
		_cherries[i]->_cherryFrameTime = rand() % 250 + 25;
		_cherries[i]->_cherryFrameCount = rand() % 1;
		_cherries[i]->_cherryCurrentFrameTime = 0;
	}

	// Initialise Ghosts
	for (i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new EnemyGhost();
		_ghosts[i]->_ghostDirection = 0;
		_ghosts[i]->_ghostSpeed = 0.2f;
		_ghosts[i]->_ghostFrameTime = 125;
		_ghosts[i]->_ghostFrameCount = 0;
		_ghosts[i]->_ghostCurrentFrameTime = 0;
	}

	_ghosts[2]->_ghostDirection = 1;

	_pacman = new Player();
	_pacman->dead = false;
	_menu = new Paused();

	_menu->_paused = false;
	_menu->_startMenuPaused = true;
	_menu->_deadScreen = false;

	_pop = new SoundEffect();
	_death = new SoundEffect();
	_backgroundFunk = new SoundEffect();
	_backgroundFunkMuffled = new SoundEffect();
	_bingBong = new SoundEffect();

	_pacman->_pacmanDirection = 0;
	_pacman->_pacmanSpeed = 0.1f;
	_pacman->_pacmanFrameTime = 250;
	_pacman->_pacmanFrameCount = 0;
	_pacman->_pacmanCurrentFrameTime = 0;
	_pacman->speedMultiplier = 1.0f;

	_pacman->_score = 0;

	/*oldMovementKey = ' ';
	tempKey = ' ';
	newMovementKey = ' ';*/

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Loops background music
	_backgroundFunk->SetLooping(true);

	// Loops muffled background music
	//_backgroundFunkMuffled->SetLooping(true);

	Audio::Play(_backgroundFunk);
	//Audio::Play(_backgroundFunkMuffled);

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();

	if (!Audio::IsInitialised())
	{
		std::cout << "Audio is not initialised" << std::endl;
	}

	if (!_pop->IsLoaded())
	{
		std::cout << "_pop member sound effect has not loaded" << std::endl;
	}

	if (!_death->IsLoaded())
	{
		std::cout << "_death member sound effect has not loaded" << std::endl;
	}

	if (!_backgroundFunk->IsLoaded())
	{
		std::cout << "_backgroundFunk member sound effect has not loaded" << std::endl;
	}

	if (!_backgroundFunkMuffled->IsLoaded())
	{
		std::cout << "_backgroundFunkMuffled member sound effect has not loaded" << std::endl;
	}
}

Pacman::~Pacman()
{
	int i;

	// Clean up pointers within the Pacman structure
	delete _pacman->_pacmanTexture;

	delete _munchies[0]->_munchieTexture;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->_munchieSourceRect;
	}

	delete _cherries[0]->_cherryTexture;

	for (i = 0; i < CHERRYCOUNT; i++)
	{
		delete _cherries[i]->_cherrySourceRect;
	}
	
	for (i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghosts[i]->_ghostSourceRect;
	}

	delete _pacman->_pacmanSourceRect;

	// Clean up the Pacman structure pointer
	delete _pacman;
	delete _munchies;
	delete _cherries;
	delete _ghosts;
	delete _menu;
	delete _pop;
	delete _death;
	delete _backgroundFunk;
	delete _bingBong;
}

void Pacman::LoadContent()
{
	int i;

	_menu->_floor = new Texture2D();
	_menu->_floor->Load("Textures/floor.png", false);

	// Load Pacman
	_pacman->_pacmanTexture = new Texture2D();
	_pacman->_pacmanTexture->Load("Textures/Pacman.png", false);
	_pacman->_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacman->_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	//Load Ghosts
	Texture2D* ghostTex = new Texture2D();
	ghostTex->Load("Textures/Ghosts.png", true);

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->_ghostTexture = ghostTex;
		_ghosts[i]->_ghostPosition = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_ghosts[i]->_ghostSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	}

	// Load Munchies
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchies.png", true);

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->_munchieTexture = munchieTex;
		_munchies[i]->_munchiePosition = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_munchies[i]->_munchieSourceRect = new Rect(0.0f, 0.0f, 12, 12);
	}
	
	// Load Cherries
	Texture2D* cherryTex = new Texture2D();
	cherryTex->Load("Textures/Cherries.png", true);

	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]->_cherryTexture = cherryTex;
		_cherries[i]->_cherryPosition = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		_cherries[i]->_cherrySourceRect = new Rect(0.0f, 0.0f, 12, 12);
	}

	// Set string position
	_menu->_stringPosition = new Vector2(10.0f, 25.0f);
	_menu->_scoreStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 25.0f);

	// Set Menu Parameters
	_menu->_menuBackground = new Texture2D();
	_menu->_menuBackground->Load("Textures/Transparency.png", false);
	_menu->_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	_pop->Load("Sounds/pop.wav");
	_death->Load("Sounds/death.wav");
	_backgroundFunk->Load("Sounds/backgroundFunk.wav");
	_backgroundFunkMuffled->Load("Sounds/backgroundFunkMuffled.wav");
	_bingBong->Load("Sounds/bingBong");


}

void Pacman::Update(int elapsedTime)
{
	int i = 0;

	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	CheckStart(keyboardState, Input::Keys::SPACE);

	CheckPaused(keyboardState, Input::Keys::P);

	CheckDead(keyboardState, Input::Keys::RETURN);

	FlickerCheck(keyboardState, Input::Keys::P);

	//MusicCheck(keyboardState, Input::Keys::P);

	/*while (_music->_musicCheck)
	{

		if (_menu->_paused)
		{
			_backgroundFunk->SetGain(-999);
			_backgroundFunkMuffled->SetGain(1);
		}
		else
		{
			_backgroundFunk->SetGain(1);
			_backgroundFunkMuffled->SetGain(-999);
		}
	}*/

	//Starts the game if it isn't paused and has started
	if ((!_menu->_paused) && (!_menu->_startMenuPaused) && (!_menu->_deadScreen)){

		//Audio::isLooping(_backgroundFunk);

		TimeFrameIncrease(elapsedTime);

		//MovementSwap();

		CheckSprint(keyboardState, Input::Keys::LEFTSHIFT);

		Input(elapsedTime, keyboardState, mouseState);

		FrameTimeCheck();

		PacmanDirectionCheck();

		UpdatePacman(elapsedTime);

		

		for (i = 0; i < GHOSTCOUNT; i++)
		{
			UpdateGhost(_ghosts[i], elapsedTime);
			//UpdateGhostMovement(_ghosts[i], elapsedTime);
			CheckGhostCollisions(_ghosts[i], elapsedTime);
		}

		UpdateGhostMovement(_ghosts[0], elapsedTime);
		UpdateGhostMovement(_ghosts[1], elapsedTime);
		UpdateGhostMovement(_ghosts[3], elapsedTime);
		EdgeWalker(_ghosts[2], elapsedTime);
		Hunting(_ghosts[4], elapsedTime);
		
		for (i = 0; i < MUNCHIECOUNT; i++)
		{
			UpdateMunchie(_munchies[i], elapsedTime);
			CheckMunchieCollisions(_munchies[i], elapsedTime);
		}
		
		for (i = 0; i < CHERRYCOUNT; i++)
		{
			UpdateCherry(_cherries[i], elapsedTime);
		}

		CheckViewportCollision();
	}
}

void Pacman::UpdateGhostMovement(EnemyGhost* ghost, int elapsedTime)
{
	float ghostSpeed = ghost->_ghostSpeed * elapsedTime;
	int changeDirection = ghost->_ghostSourceRect->Y = ghost->_ghostSourceRect->Height * ghost->_ghostDirection;

	if (ghost->_ghostDirection == 0) // Moves right
	{
		ghost->_ghostPosition->X += ghostSpeed;
		changeDirection;
	}
	else if (ghost->_ghostDirection == 1) // Moves down
	{
		ghost->_ghostPosition->Y += ghostSpeed;
		changeDirection;
	}
	else if (ghost->_ghostDirection == 2) // Moves left
	{
		ghost->_ghostPosition->X -= ghostSpeed;
		changeDirection;
	}
	else if (ghost->_ghostDirection == 3) // Moves up
	{
		ghost->_ghostPosition->Y -= ghostSpeed;
		changeDirection;
	}

	if (ghost->_ghostPosition->X + ghost->_ghostSourceRect->Width >= Graphics::GetViewportWidth()) // Hits right edge
	{
		ghost->_ghostDirection = 2; // Change direction
	}
	else if (ghost->_ghostPosition->X <= 0) // Hits left edge
	{
		ghost->_ghostDirection = 0; // Change direction
	}
	else if (ghost->_ghostPosition->Y + ghost->_ghostSourceRect->Height >= Graphics::GetViewportHeight()) // Hits bottom edge
	{
		ghost->_ghostDirection = 3;
	}
	else if (ghost->_ghostPosition->Y <= 0) // Hits top edge
	{
		ghost->_ghostDirection = 1;
	}
}

void Pacman::Hunting(EnemyGhost* ghost, int elapsedTime)
{
	if (ghost->_ghostPosition->X < _pacman->_pacmanPosition->X)
	{
		ghost->_ghostPosition->X += ghost->_ghostSpeed * 7; // Increase the ghosts speed
	}
	else if (ghost->_ghostPosition->X > _pacman->_pacmanPosition->X)
	{
		ghost->_ghostPosition->X -= ghost->_ghostSpeed * 7;
	}
	if (ghost->_ghostPosition->Y < _pacman->_pacmanPosition->Y)
	{
		ghost->_ghostPosition->Y += ghost->_ghostSpeed * 7;
	}
	else if (ghost->_ghostPosition->Y > _pacman->_pacmanPosition->Y)
	{
		ghost->_ghostPosition->Y -= ghost->_ghostSpeed * 7;
	}
}

void Pacman::EdgeWalker(EnemyGhost* ghost, int elapsedTime)
{
	float ghostSpeed = ghost->_ghostSpeed * elapsedTime;
	int changeDirection = ghost->_ghostSourceRect->Y = ghost->_ghostSourceRect->Height * ghost->_ghostDirection;

	if (ghost->_ghostDirection == 0) // Moves right
	{
		ghost->_ghostPosition->X += ghostSpeed;
		changeDirection;
	}
	else if (ghost->_ghostDirection == 1) // Moves down
	{
		ghost->_ghostPosition->Y += ghostSpeed;
		changeDirection;
	}
	else if (ghost->_ghostDirection == 2) // Moves left
	{
		ghost->_ghostPosition->X -= ghostSpeed;
		changeDirection;
	}
	else if (ghost->_ghostDirection == 3) // Moves up
	{
		ghost->_ghostPosition->Y -= ghostSpeed;
		changeDirection;
	}

	if (ghost->_ghostPosition->X + ghost->_ghostSourceRect->Width + 30 >= Graphics::GetViewportWidth()) // Hits right edge
	{
		ghost->_ghostDirection = 3; // Go up
	}
	else if (ghost->_ghostPosition->X - 30 <= 0) // Hits left edge
	{
		ghost->_ghostDirection = 1; // Go down
	}
	else if (ghost->_ghostPosition->Y + ghost->_ghostSourceRect->Height + 30 >= Graphics::GetViewportHeight()) // Hits bottom edge
	{
		ghost->_ghostDirection = 0; // Go right
	}
	else if (ghost->_ghostPosition->Y - 40 <= 0) // Hits top edge
	{
		ghost->_ghostDirection = 2; // Go left
	}

	if ((ghost->_ghostPosition->X + ghost->_ghostSourceRect->Width + 30 >= Graphics::GetViewportWidth()) && (ghost->_ghostPosition->Y + ghost->_ghostSourceRect->Height + 30 >= Graphics::GetViewportHeight())) // Hits bottom right
	{
		ghost->_ghostDirection = 3; // Go up
	}
	else if ((ghost->_ghostPosition->X + ghost->_ghostSourceRect->Width + 30 >= Graphics::GetViewportWidth()) && (ghost->_ghostPosition->Y + ghost->_ghostSourceRect->Height - 40 <= 0)) // Hits top right
	{
		ghost->_ghostDirection = 2; // Go left
	}
	else if ((ghost->_ghostPosition->X + ghost->_ghostSourceRect->Width - 30 <= 0) && (ghost->_ghostPosition->Y + ghost->_ghostSourceRect->Height - 40 <= 0)) // Hits top left
	{
		ghost->_ghostDirection = 1; // Go down
	}
	else if ((ghost->_ghostPosition->X + ghost->_ghostSourceRect->Width - 30 <= 0) && (ghost->_ghostPosition->Y + ghost->_ghostSourceRect->Height + 30 >= Graphics::GetViewportHeight())) // Hits bottom left
	{
		ghost->_ghostDirection = 0; // Go right
	}
	else if (ghost->_ghostPosition->Y + ghost->_ghostSourceRect->Height + 30 >= Graphics::GetViewportHeight()) // Hits bottom edge
	{
		ghost->_ghostDirection = 0; // Go right
	}
}

void Pacman::Draw(int elapsedTime)
{
	int i;

	// Allows us to easily create a string
	stringstream stream;
	stringstream scoreStream;
	
	stream << "Pacman X: " << _pacman->_pacmanPosition->X << " Y: " << _pacman->_pacmanPosition->Y;
	scoreStream << "Score: " << _pacman->_score;
	
	SpriteBatch::BeginDraw(); // Starts Drawing

	SpriteBatch::Draw(_menu->_floor, _menu->_menuRectangle, nullptr);

	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->_pacmanTexture, _pacman->_pacmanPosition, _pacman->_pacmanSourceRect); // Draws Pacman
	}
	
	for (i = 0; i < GHOSTCOUNT; i++) // Draws all the ghosts
	{
		SpriteBatch::Draw(_ghosts[i]->_ghostTexture, _ghosts[i]->_ghostPosition, _ghosts[i]->_ghostSourceRect);
	}

	for (i = 0; i < MUNCHIECOUNT; i++) // Draws all the munchies
	{
		SpriteBatch::Draw(_munchies[i]->_munchieTexture, _munchies[i]->_munchiePosition, _munchies[i]->_munchieSourceRect);
	}
	
	for (i = 0; i < CHERRYCOUNT; i++) // Draws all the cherries
	{
		SpriteBatch::Draw(_cherries[i]->_cherryTexture, _cherries[i]->_cherryPosition, _cherries[i]->_cherrySourceRect);
	}
	
	if (_menu->_startMenuPaused)
	{
		std::stringstream menuStream;
		menuStream << "Press SPACE to start";

		SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Green);
	}

	if (_menu->_paused)
	{
		std::stringstream menuStream;
		menuStream << "Game Paused";

		SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Magenta);
	}

	if (_menu->_deadScreen)
	{
		std::stringstream menuStream;
		menuStream << "You Died. \n Press Enter to try again";

		SpriteBatch::Draw(_menu->_menuBackground, _menu->_menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->_menuStringPosition, Color::Red);
	}
	

	SpriteBatch::DrawString(stream.str().c_str(), _menu->_stringPosition, Color::Yellow);
	SpriteBatch::DrawString(scoreStream.str().c_str(), _menu->_scoreStringPosition, Color::Blue);

	SpriteBatch::EndDraw(); // Ends Drawing
}

//Specific variables below

void Pacman::TimeFrameIncrease(int elapsedTime)
{
	int i;
	// Increaces Pacmans, the Munchies and the Cherries current time frame
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->_munchieCurrentFrameTime += elapsedTime;
	}
	
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]->_cherryCurrentFrameTime += elapsedTime;
	}
	
	for (i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->_ghostCurrentFrameTime += elapsedTime;
	}

	_pacman->_pacmanCurrentFrameTime += elapsedTime;
}

//void Pacman::MovementSwap()
//{
//	oldMovementKey = newMovementKey;
//	newMovementKey = ' ';
//}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	float pacmanSpeed = _pacman->_pacmanSpeed * elapsedTime * _pacman->speedMultiplier;

	if (mouseState->LeftButton == Input::ButtonState::PRESSED)
	{
		int tempPos = rand() % 25;
		_cherries[tempPos]->_cherryPosition->X = mouseState->X;
		_cherries[tempPos]->_cherryPosition->Y = mouseState->Y;

	}

	// Checks if W key is pressed
	if ((state->IsKeyDown(Input::Keys::W)))// && (oldMovementKey !='W'))
	{
		if ((state->IsKeyUp(Input::Keys::A)) && (state->IsKeyUp(Input::Keys::S)) && (state->IsKeyUp(Input::Keys::D)))
		{
			//newMovementKey = 'W';
			_pacman->_pacmanPosition->Y -= pacmanSpeed; // Moves Pacman negatively across the Y axis
			_pacman->_pacmanDirection = 3;
		}
	}

	// Checks if A key is pressed
	else if ((state->IsKeyDown(Input::Keys::A)))// && (oldMovementKey != 'A'))
	{
		if ((state->IsKeyUp(Input::Keys::W)) && (state->IsKeyUp(Input::Keys::S)) && (state->IsKeyUp(Input::Keys::D)))
		{
			//newMovementKey = 'A';
			_pacman->_pacmanPosition->X -= pacmanSpeed; // Moves Pacman negatively across the X axis
			_pacman->_pacmanDirection = 2;
		}
	}

	// Checks if S key is pressed
	else if ((state->IsKeyDown(Input::Keys::S)))// && (oldMovementKey != 'S'))
	{
		if ((state->IsKeyUp(Input::Keys::W)) && (state->IsKeyUp(Input::Keys::A)) && (state->IsKeyUp(Input::Keys::D)))
		{
			//newMovementKey = 'S';
			_pacman->_pacmanPosition->Y += pacmanSpeed; // Moves Pacman positively across the Y axis
			_pacman->_pacmanDirection = 1;
		}
	}

	// Checks if D key is pressed
	else if ((state->IsKeyDown(Input::Keys::D)))// && (oldMovementKey != 'D'))
	{
		if ((state->IsKeyUp(Input::Keys::W)) && (state->IsKeyUp(Input::Keys::A)) && (state->IsKeyUp(Input::Keys::S)))
		{
			//newMovementKey = 'D';
			_pacman->_pacmanPosition->X += pacmanSpeed; // Moves Pacman positively across X axis
			_pacman->_pacmanDirection = 0;
		}
	}

	//MovementSwap();

}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// Pauses the whole game if the P key is pressed
	if (state->IsKeyDown(Input::Keys::P) && !_menu->_pKeyDown)
	{
		if (_menu->_startMenuPaused == false)
		{
			_menu->_pKeyDown = true;
			_menu->_paused = !_menu->_paused;
		}
	}
}

void Pacman::CheckDead(Input::KeyboardState* state, Input::Keys enterKey)
{
	if (_pacman->dead == true)
		_menu->_deadScreen = true;
	if (state->IsKeyDown(Input::Keys::RETURN))
		_menu->_deadScreen = false;
		_pacman->dead = false;
}

void Pacman::CheckStart(Input::KeyboardState* state, Input::Keys startKey)
{
	// Start Menu Screen
	if (state->IsKeyDown(Input::Keys::SPACE))
	{
		_menu->_startMenuPaused = false;
	}
}

void Pacman::CheckSprint(Input::KeyboardState* state, Input::Keys shiftKey)
{
	if (state->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		// Apply multiplier
		_pacman->speedMultiplier = 2.0f;
	}
	else
	{
		// Reset multiplier
		_pacman->speedMultiplier = 1.0f;
	}
}

void Pacman::FlickerCheck(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// If P is pressed then it isn't down so it stoppes flickering
	if (state->IsKeyUp(Input::Keys::P))
		_menu->_pKeyDown = false;
}

//void Pacman::MusicCheck(Input::KeyboardState* state, Input::Keys pauseKey)
//{
//	if ((!_menu->_paused) && ());
//		_music->_musicCheck = true;
//}

void Pacman::FrameTimeCheck()
{
	int i;
	// Changes Pacmans mouth to open in accordance to the frame time
	_pacman->_pacmanSourceRect->X = _pacman->_pacmanSourceRect->Width * _pacman->_pacmanFrameCount;

	// Changes the Munchies and Cherries texture to swap in accordance to the frame time
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->_munchieSourceRect->Y = _munchies[i]->_munchieSourceRect->Height * _munchies[i]->_munchieFrameCount;
	}

	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]->_cherrySourceRect->Y = _cherries[i]->_cherrySourceRect->Height * _cherries[i]->_cherryFrameCount;
	}

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]-> _ghostSourceRect->X = _ghosts[i]->_ghostSourceRect->Width * _ghosts[i]->_ghostFrameCount;
	}
	
}

void Pacman::PacmanDirectionCheck()
{
	// Changes the direction Pacman is facing in accordance to his direction of travel
	_pacman->_pacmanSourceRect->Y = _pacman->_pacmanSourceRect->Height * _pacman->_pacmanDirection;
}

void Pacman::UpdatePacman(int elapsedTime)
{
	// Updates the variable by adding 1 then resetting if greater than 2 which means the Pacman frames are now time dependant and will run at the same speed on all machines
	if (_pacman->_pacmanCurrentFrameTime > _pacman->_pacmanFrameTime)
	{
		_pacman->_pacmanFrameCount++;

		if (_pacman->_pacmanFrameCount >= 2)
			_pacman->_pacmanFrameCount = 0;
		_pacman->_pacmanCurrentFrameTime = 0;
	}
}

void Pacman::UpdateMunchie(FoodMunchie* munchie, int elapsedTime)
{
	// Updates the variable by adding 1 then resetting if greater than 2 which means the Munchie frames are now time dependant and will run at the same speed on all machines
		if (munchie->_munchieCurrentFrameTime > munchie->_munchieFrameTime)
		{
			munchie->_munchieFrameCount++;

			if (munchie->_munchieFrameCount >= 2) {
				munchie->_munchieFrameCount = 0;
			}
			munchie->_munchieCurrentFrameTime = 0;
		}
}

void Pacman::UpdateCherry(FoodCherry* cherry, int elapsedTime)
{
	// Updates the variable by adding 1 then resetting if greater than 2 which means the Cherry frames are now time dependant and will run at the same speed on all machines
	if (cherry->_cherryCurrentFrameTime > cherry->_cherryFrameTime)
	{
		cherry->_cherryFrameCount++;

		if (cherry->_cherryFrameCount >= 2) {
			cherry->_cherryFrameCount = 0;
		}
		cherry->_cherryCurrentFrameTime = 0;
	}
}

void Pacman::UpdateGhost(EnemyGhost* ghost, int elapsedTime)
{
	// Updates the variable by adding 1 then resetting if greater than 2 which means the Cherry frames are now time dependant and will run at the same speed on all machines
	if (ghost->_ghostCurrentFrameTime > ghost->_ghostFrameTime)
	{
		ghost->_ghostFrameCount++;

		if (ghost->_ghostFrameCount >= 2) {
			ghost->_ghostFrameCount = 0;
		}
		ghost->_ghostCurrentFrameTime = 0;
	}
}

void Pacman::CheckViewportCollision()
{
	int right = _pacman->_pacmanPosition->X;// + _pacman->_pacmanSourceRect->Width;
	int left = _pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width;
	int up = _pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Height;
	int down = _pacman->_pacmanPosition->Y;// + _pacman->_pacmanSourceRect->Height;

	// Checks if Pacman is tyrying to disappear off to the right side of the screen
	if (right > Graphics::GetViewportWidth()) //1024 is game screen width
	{
		// Pacman hit right wall and then gets put round to the other side of the screen exactly
		_pacman->_pacmanPosition->X = 0 -_pacman->_pacmanSourceRect->Width;
	}

	// Checks if Pacman is tyrying to disappear off to the left side of the screen
	if (left < 0) //0 is the left most side of the screen so if his position in ANY way is below
	{
		// Pacman hit left wall and then gets put round to the other side of the screen with - his width or he would appear off the screen
		_pacman->_pacmanPosition->X = Graphics::GetViewportWidth();
	}

	// Checks if Pacman is tyrying to disappear off to the top side of the screen
	if (up < 0) // 0 is the top of the screen so if his position in ANY way is above
	{
		// Pacman hit top wall and then gets put round to the other side of the screen - his height or he would appear off the screen
		_pacman->_pacmanPosition->Y = Graphics::GetViewportHeight();
	}

	// Checks if Pacman is tyrying to disappear off to the bottom side of the screen
	if (down > Graphics::GetViewportHeight()) //768 is game screen height
	{
		// Pacman hit bottom wall and then gets put round to the other side of the screen exactly
		_pacman->_pacmanPosition->Y = 0 - _pacman->_pacmanSourceRect->Height;
	}
}

void Pacman::CheckGhostCollisions(EnemyGhost* ghost, int elapsedTime)
{
	int i = 0;
	int bottom1 = _pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->_pacmanPosition->X;
	int left2 = 0;
	int right1 = _pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->_pacmanPosition->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		// Populate variables with Ghost data
		bottom2 = _ghosts[i]->_ghostPosition->Y + _ghosts[i]->_ghostSourceRect->Height;
		left2 = _ghosts[i]->_ghostPosition->X;
		right2 = _ghosts[i]->_ghostPosition->X + _ghosts[i]->_ghostSourceRect->Width;
		top2 = _ghosts[i]->_ghostPosition->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			Audio::Play(_death);
			_pacman->dead = true;
			_menu->_deadScreen = false;
			i = GHOSTCOUNT;
		}
	}

}

void Pacman::CheckMunchieCollisions(FoodMunchie* munchie, int elapsedTime)
{
	int i = 0;
	int bottom1 = _pacman->_pacmanPosition->Y + _pacman->_pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->_pacmanPosition->X;
	int left2 = 0;
	int right1 = _pacman->_pacmanPosition->X + _pacman->_pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->_pacmanPosition->Y;
	int top2 = 0;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		// Populate variables with Munchie data
		bottom2 = _munchies[i]->_munchiePosition->Y + _munchies[i]->_munchieSourceRect->Height;
		left2 = _munchies[i]->_munchiePosition->X;
		right2 = _munchies[i]->_munchiePosition->X + _munchies[i]->_munchieSourceRect->Width;
		top2 = _munchies[i]->_munchiePosition->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_munchies[i]->_munchiePosition->X = -999;
			_pacman->_score += 1;
			Audio::Play(_pop);
			/*delete _munchies[i]->_munchieTexture;
			delete _munchies[i]->_munchieSourceRect;*/
			i = MUNCHIECOUNT;
		}
	}

}

//void Audio::SetLooping()
//{
//
//}