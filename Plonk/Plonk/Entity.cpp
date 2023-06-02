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

Entity::Entity(class Game *game, const char *name) : game(game), name(name) {}
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

void Entity::SetRect(SDL_Rect &rect) {
	const std::type_info* cid;
	Component *component;

	cid = &typeid(PositionComponent);
	component = (*this)[cid];
	PositionComponent* pc = static_cast<PositionComponent*>(component);

	cid = &typeid(ShapeComponent);
	component = (*this)[cid];
	ShapeComponent* sc = static_cast<ShapeComponent*>(component);

	rect.w = sc->w();
	rect.h = sc->h();
	rect.x = pc->x() - rect.w / 2;
	rect.y = pc->y() - rect.h / 2;
}
