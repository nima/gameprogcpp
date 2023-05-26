// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.hpp"

int main(int argc, char** argv) {
	Game game;
	game.Initialize() ? game.RunLoop() : game.Shutdown();
	return 0;
}
