// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>
#include "SDL/SDL.h"

// Vector2 struct just stores x/y coordinates
// (for now)
struct Vector2 {
	float x;
	float y;
};

struct Paddle {
	Vector2 pos;
	int dir;
	int kUp;
	int kDown;
	int colXMin; // Zone along the x-axis in which collision is activated in
	int colXMax; // Zone along the x-axis in which collision is activated in
};

struct Ball {
	Vector2 pos;
	Vector2 vel;
};

// Game class
class Game {
public:
	Game();
	// Initialize the game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();
    
private:
	// Helper functions for the game loop
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;
	
	// Pong specific
	const int winWidth;
	const int winHeight;
	
	SDL_Rect tWall;
	SDL_Rect net;
	SDL_Rect bWall;

	// The Paddles
	std::vector<Paddle> paddles;
	
	// The Ball
	std::vector<Ball> balls;
};
