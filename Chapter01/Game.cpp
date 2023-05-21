// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <math.h>

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game():
    mWindow(nullptr),
    mRenderer(nullptr),
    mTicksCount(0),
    mIsRunning(true),
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
	
	paddles = {
		Paddle{
			Vector2{10.0f, winWidth/2.0f},
			0,
			SDL_SCANCODE_W, SDL_SCANCODE_S,
			20, 25,
		},
		Paddle{
			Vector2{winWidth - (10.0f + thickness), winHeight - winWidth/2.0f},
			0,
			SDL_SCANCODE_I, SDL_SCANCODE_K,
			-25, -20,
		}
	};

	balls = {
		Ball{ Vector2{winHeight/2.0f, winWidth/2.0f}, Vector2{-200.0f, 235.0f}},
		Ball{ Vector2{winWidth - winHeight/2.0f, winHeight - winWidth/2.0f}, Vector2{100.0f, 20.0f}},
	};

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
	
	for (auto &paddle: paddles) {
		// Update left paddle direction based on user key-presses
		paddle.dir = 0;
		if (state[paddle.kUp]) paddle.dir -= 1;
		if (state[paddle.kDown]) paddle.dir += 1;
	}
}

void Game::UpdateGame() {
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from last frame (converted to seconds)
	// Clamp maximum delta time value to 0.05
	float deltaTime = fmin(0.05f, (SDL_GetTicks() - mTicksCount) / 1000.0f);

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();
	
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
	}

	// Update ball position based on ball velocity
	for (auto &ball: balls) {
		ball.pos.x += ball.vel.x * deltaTime;
		ball.pos.y += ball.vel.y * deltaTime;
	
		// Bounce if needed
		for (const auto &paddle: paddles) {
			float diff = fabs(paddle.pos.y - ball.pos.y);
			float x1 = paddle.colXMin;
			float x2 = paddle.colXMax;
			if (x1 < 0) x1 += winWidth;
			if (x2 < 0) x2 += winWidth;
			
			if (diff >= paddleH / 2.0f) continue;
			
			const bool opIsLessThan = ball.pos.x < winWidth/2;
			if (ball.pos.x >= x1 && ball.pos.x <= x2 && (opIsLessThan ? (ball.vel.x < 0.0f) : (ball.vel.x > 0.0f))) {
				// Did we intersect with the paddle?
				// Our y-difference is small enough
				// We are in the correct x-position
				// The ball is moving to the left
				ball.vel.x *= -1.0f;
			} else if (ball.pos.x <= 0.0f || ball.pos.x >= winWidth) {
				// Did the ball go off the screen? (if so, end game)
				mIsRunning = false;
			}
		}
	
		if (ball.pos.y <= thickness && ball.vel.y < 0.0f) {
			// Did the ball collide with the top wall?
			ball.vel.y *= -1;
		} else if (ball.pos.y >= (winHeight - thickness) && ball.vel.y > 0.0f) {
			// Did the ball collide with the bottom wall?
			ball.vel.y *= -1;
		}
	}
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
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
