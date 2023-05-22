//
//  Entity.cpp
//  Game-mac
//
//  Created by Nima Talebi on 5/21/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//

#include <typeindex>
#include <typeinfo>
#include <iostream>

#include <stdio.h>

#include "Entity.hpp"

Entity::Entity(class Game *game) {
	this->game = game;
}
Entity::~Entity() {}

Component* Entity::operator[](const std::type_info* cid) {
	return this->components[cid];
}

void Entity::Update(float dt) {}

void Entity::AddComponent(class Component *component) {
	this->components[component->TypeId()] = component;
}

void Entity::DelComponent(class Component *component) {}
void Entity::UpdateComponents(float dt) {}
