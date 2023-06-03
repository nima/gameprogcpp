//
//  Component.cpp
//  Game-mac
//
//  Created by Nima Talebi on 5/21/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#include "Component.hpp"

#include <box2d/b2_types.h>

#include "Entity.hpp"

#include <iostream>
#include <SDL2/SDL_log.h>

Component::Component(class Entity* owner) {
	this->owner = owner;
}

ControllerComponent::ControllerComponent(Entity *owner, float speed, tController controller) : Component(owner) {
	this->speed = speed;
	this->controller = controller;
}

ShapeComponent::ShapeComponent(Entity *owner, tDimensions dimensions) : Component(owner) {
	this->dimensions = dimensions;
}

PositionComponent::PositionComponent(Entity *owner, b2World &world, tPosition position, tBoundary boundary) : Component(owner), world(world)  {
	this->position = position;
	this->boundary = boundary;
	
	// Create a Box2D body for the entity
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; // The body will move in response to forces
	bodyDef.position.Set(this->position.x, this->position.y); // Set the initial position of the body
	this->body = world.CreateBody(&bodyDef);
}

void PositionComponent::UpdatePosition(ShapeComponent *sc, float dx, float dy) {
	this->position.y += dy;
	if (this->position.y < (sc->h()/2.0f + sc->w())) {
		this->position.y = sc->h()/2.0f + sc->w();
	} else if (this->position.y > (this->boundary.yN - sc->h()/2.0f)) {
		this->position.y = this->boundary.yN - sc->h()/2.0f;
	}

	this->position.x += dx;
}

VelocityComponent::VelocityComponent(Entity *owner, b2World &world, tVelocity velocity) : Component(owner), world(world)  {
	this->velocity = velocity;
}

AccelerationComponent::AccelerationComponent(Entity *owner, b2World &world) : Component(owner), world(world) {
	/*
	Component* component;
	const std::type_info* cid;
	Entity *entity = this->owner;

	cid = &typeid(PositionComponent);
	component = (*entity)[cid];
	PositionComponent* pc = static_cast<PositionComponent*>(component);
	*/
}
