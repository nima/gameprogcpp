// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <math.h>

#include "Game.hpp"

const float paddleH = 100.0f;
const float paddleW = 20.0f;

Game::Game():
    mTicksCount(0),
    mIsRunning(true),
	mWindow(nullptr),
	mRenderer(nullptr),
	winWidth(1200),
	winHeight(800)
{}

bool Game::Initialize() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)", 100, 100, winWidth, winHeight, 0);
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	// Create SDL renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	
	this->registry = new Registry();
	this->registry->Register(new InputSystem());
	this->registry->Register(new VelocitySystem());
	this->registry->Register(new CollisionSystem());
	this->registry->Register(new RenderSystem(mRenderer));

	Entity *entity;
	tBoundary screenBoundary = {0, 0, winWidth, winHeight};
	tBoundary lPaddleBoundary = {10, 20, 50, winHeight-20};
	tBoundary rPaddleBoundary = {winWidth-50, 20, winWidth-10, winHeight-20};

	// Left Paddle
	entity = new Entity(this);
	entity->AddComponent(new ControllerComponent(entity, 10.0f, tController{SDL_SCANCODE_W, SDL_SCANCODE_S}));
	entity->AddComponent(new PositionComponent(entity, tPosition{10.0f+paddleW/2.0f, winWidth/2.0f}, lPaddleBoundary));
	entity->AddComponent(new ShapeComponent(entity, tDimensions{paddleW, paddleH}));
	entity->AddComponent(new PhysicsComponent(entity, tCollisionZoneX{20.0f, 25.0f}));
	this->registry->Register(entity);

	// Right Paddle
	entity = new Entity(this);
	entity->AddComponent(new ControllerComponent(entity, 10.0f, tController{SDL_SCANCODE_I, SDL_SCANCODE_K}));
	entity->AddComponent(new PositionComponent(entity, tPosition{winWidth - (10.0f + paddleW/2.0f), winHeight - winWidth/2.0f}, rPaddleBoundary));
	entity->AddComponent(new ShapeComponent(entity, tDimensions{paddleW, paddleH}));
	entity->AddComponent(new PhysicsComponent(entity, tCollisionZoneX{-25, -20}));
	this->registry->Register(entity);

	// Ball #1
	entity = new Entity(this);
	entity->AddComponent(new PositionComponent(entity, tPosition{winHeight/2.0f, winWidth/2.0f}, screenBoundary));
	entity->AddComponent(new ShapeComponent(entity, tDimensions{15.0, 15.0}));
	entity->AddComponent(new MobileComponent(entity, tVelocity{-200.0f, 235.0f}));
	this->registry->Register(entity);

	// Ball #1
	entity = new Entity(this);
	entity->AddComponent(new PositionComponent(entity, tPosition{winWidth - winHeight/2.0f, winHeight - winWidth/2.0f}, screenBoundary));
	entity->AddComponent(new ShapeComponent(entity, tDimensions{15.0, 15.0}));
	entity->AddComponent(new MobileComponent(entity, tVelocity{100.0f, 20.0f}));
	this->registry->Register(entity);
	
	const int thickness = 10;
	tWall = SDL_Rect{0, 0, winWidth, thickness};
	bWall = SDL_Rect{0, winHeight-thickness, winWidth, thickness};
	net = SDL_Rect{static_cast<int>(winWidth - thickness)/2, 0, thickness, winHeight};
	
	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	this->registry->ProcessInput(state);
}

void Game::UpdateGame() {
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from last frame (converted to seconds)
	// Clamp maximum delta time value to 0.05
	float dt = fmin(0.05f, (SDL_GetTicks() - mTicksCount) / 1000.0f);

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	this->registry->UpdateGame(dt);
	/*
	// Update paddle position based on direction
	for (auto &paddle: paddles) {
		if (paddle.dir == 0) continue;
		
		// Move the paddle
		paddle.pos.y += paddle.dir * 300.0f * deltaTime;

		// Make sure paddle doesn't move off screen!
		if (paddle.pos.y < (paddleH/2.0f + thickness)) {
			paddle.pos.y = paddleH/2.0f + thickness;
		} else if (paddle.pos.y > (winHeight - paddleH/2.0f - thickness)) {
			paddle.pos.y = winHeight - paddleH/2.0f - thickness;
		}
	}*/
}

void Game::GenerateOutput() {
	// Set draw color to blue (in RGBA)
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0xff, 0xff);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 0xff, 0xff, 0xff, 0xff);
	
	// Draw the two walls and the net
	SDL_RenderFillRect(mRenderer, &tWall);
	SDL_RenderFillRect(mRenderer, &bWall);
	SDL_RenderFillRect(mRenderer, &net);

	this->registry->GenerateOutput();
	/*
	// Draw paddle
	SDL_Rect lPaddle{
		static_cast<int>(paddles[0].pos.x),
		static_cast<int>(paddles[0].pos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &lPaddle);

	SDL_Rect rPaddle = {
		static_cast<int>(paddles[1].pos.x),
		static_cast<int>(paddles[1].pos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &rPaddle);

	// Draw balls
	for (const auto &ball: balls) {
		SDL_Rect ballRect{
			static_cast<int>(ball.pos.x - thickness/2),
			static_cast<int>(ball.pos.y - thickness/2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ballRect);
	}
	*/
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
