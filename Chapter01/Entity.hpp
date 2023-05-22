//
//  Paddle.h
//  Game-mac
//
//  Created by Nima Talebi on 5/21/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#pragma once

#include <map>

#include "Component.hpp"

struct TypeInfoComparator {
	bool operator()(const std::type_info* lhs, const std::type_info* rhs) const {
		return lhs->before(*rhs);
	}
};

class Entity {
private:
	class Game *game;
	std::map<const std::type_info*, class Component*, TypeInfoComparator> components;
public:
	Entity(class Game *game);
	~Entity();
	
	Component* operator[](const std::type_info* cid);

	void Update(float dt);
	void UpdateComponents(float dt);
	//virtual void UpdateComponents(const Uint8 *state);
	//virtual void UpdateEntity(float dt);

	void AddComponent(class Component *component);
	void DelComponent(class Component *component);
};
