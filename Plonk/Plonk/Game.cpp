// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.hpp"

#include <math.h>

const float paddleH = 100.0f;
const float paddleW = 20.0f;

Game::Game():
    mTicksCount(0),
    mIsRunning(true),
	mWindow(nullptr),
	mRenderer(nullptr),
	winWidth(1200),
	winHeight(800),
	world(b2Vec2{0.0f, 0.0f})
{}

b2Body *Game::mkPaddle(float x, float y) {
	world.SetAutoClearForces(false);
	
	b2PolygonShape shape;
	shape.SetAsBox(paddleW, paddleH);
	
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	bodyDef.gravityScale = 1.0;
	b2Body *body = this->world.CreateBody(&bodyDef);
	
	b2FixtureDef b2fd;
	b2fd.shape = &shape;
	body->CreateFixture(&b2fd);
	
	b2Vec2 b2vCenter(0.0f, 0.0f);
	b2MassData *b2md = new b2MassData();
	b2md->mass = 1.0f;
	body->SetMassData(b2md);
	
	return body;
}

b2Body *Game::mkBall(float x, float y) {
	//b2CircleShape shape;
	//shape.m_radius = 150;
	b2PolygonShape shape;
	shape.SetAsBox(paddleW/2.0f, paddleW/2.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x, y);
	b2Body *body = this->world.CreateBody(&bodyDef);
	
	b2FixtureDef b2fd;
	b2fd.shape = &shape;
	body->CreateFixture(&b2fd);
	
	return body;
}


bool Game::Initialize() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow("Plonk", 100, 100, winWidth, winHeight, 0);
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
	this->registry->Register(new PhysicsSystem(world));
	this->registry->Register(new CollisionSystem());
	this->registry->Register(new RenderSystem(mRenderer));
	
	Entity *entity;
	b2Body *body;
	
	tBoundary lPaddleBoundary = {10, 20, 50, winHeight-20};
	entity = new Entity(this, "lPaddle");
	entity->AddComponent(new ControllerComponent(entity, 200.0f, tController{SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D}));
	body = mkPaddle(10.0f+paddleW/2.0f, winWidth/2.0f);
	entity->AddComponent(new PhysicalComponent(entity, world, body, lPaddleBoundary));
	this->registry->Register(entity);

	tBoundary rPaddleBoundary = {winWidth-50, 20, winWidth-10, winHeight-20};
	entity = new Entity(this, "rPaddle");
	entity->AddComponent(new ControllerComponent(entity, 200.0f, tController{SDL_SCANCODE_I, SDL_SCANCODE_K}));
	body = mkPaddle(winWidth - (10.0f + paddleW/2.0f), winHeight - winWidth/2.0f);
	entity->AddComponent(new PhysicalComponent(entity, world, body, rPaddleBoundary));
	this->registry->Register(entity);

	tBoundary screenBoundary = {0, 0, winWidth, winHeight};
	entity = new Entity(this, "ball");
	body = mkBall(winHeight/2.0f, winWidth/3.0f);
	entity->AddComponent(new PhysicalComponent(entity, world, body, screenBoundary));
	this->registry->Register(entity);
	
	/*
	const float wallThickness = 10.0f;
	 
	entity = new Entity(this, "wallTop");
	entity->AddComponent(new PositionComponent(entity, world, tPosition{winWidth/2.0f, wallThickness/2.0f}, screenBoundary));
	entity->AddComponent(new ShapeComponent(entity, tDimensions{winWidth/1.0f, wallThickness}));
	this->registry->Register(entity);

	entity = new Entity(this, "wallBottom");
	entity->AddComponent(new PositionComponent(entity, world, tPosition{winWidth/2.0f, winHeight-wallThickness/2.0f}, screenBoundary));
	entity->AddComponent(new ShapeComponent(entity, tDimensions{winWidth/1.0f, wallThickness}));
	this->registry->Register(entity);

	entity = new Entity(this, "net");
	entity->AddComponent(new PositionComponent(entity, world, tPosition{winWidth/2.0f, winHeight/2.0f}, screenBoundary));
	entity->AddComponent(new ShapeComponent(entity, tDimensions{3.0f, winHeight/1.1f}));
	this->registry->Register(entity);
	*/
	
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
}

void Game::GenerateOutput() {
	// Set draw color to blue (in RGBA)
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0xff, 0xff);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 0xff, 0xff, 0xff, 0xff);
	
	this->registry->GenerateOutput();
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

