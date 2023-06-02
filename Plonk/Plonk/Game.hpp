// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------
#pragma once

#include <vector>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "Registry.hpp"
#include "Entity.hpp"
#include "System.hpp"
#include "ControllerComponent.hpp"

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

	Registry *registry;
	
	// Number of ticks since start of game
	Uint32 mTicksCount;
	// Game should continue to run
	bool mIsRunning;

	// Window created by SDL
	SDL_Window* mWindow;
	// Renderer for 2D drawing
	SDL_Renderer* mRenderer;
	
	// Pong specific
	const int winWidth;
	const int winHeight;

	std::vector<Entity*> entities;

	b2World world;
};
