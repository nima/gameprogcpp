//
//  Paddle.h
//  Game-mac
//
//  Created by Nima Talebi on 5/21/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#pragma once

#include <map>

#include <SDL2/SDL_rect.h>

#include "Component.hpp"

struct TypeInfoComparator {
	bool operator()(const std::type_info* lhs, const std::type_info* rhs) const {
		return lhs->before(*rhs);
	}
};

class Entity {
private:
	class Game *game;
	const char *name;
	std::map<const std::type_info*, class Component*, TypeInfoComparator> components;
public:
	Entity(class Game *game, const char *name);
	~Entity();
	
	const char* Name() { return this->name; };
	
	Component* operator[](const std::type_info* cid);

	void Update(float dt);
	void UpdateComponents(float dt);
	void SetRect(SDL_Rect &rect);
	
	void AddComponent(class Component *component);
	void DelComponent(class Component *component);
};
