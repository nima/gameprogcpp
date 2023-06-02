//
//  Registry.hpp
//  Game-mac
//
//  Created by Nima Talebi on 5/23/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#pragma once

#include <map>
#include <vector>
#include <string>

#include "System.hpp"
#include "Entity.hpp"

class Registry {
public:
	Registry();
	~Registry();
	void Register(System *);
	void Register(Entity *);
	void ProcessInput(const Uint8*);
	void UpdateGame(float dt);
	void GenerateOutput();
private:
	std::vector<System*> systems;
	std::vector<Entity*> entity;
};
