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

PhysicalComponent::PhysicalComponent(Entity *owner, b2World &world, b2Body *body, tBoundary boundary) : Component(owner), world(world), body(body) {
	this->boundary = boundary;	
}
PhysicalComponent::~PhysicalComponent() {}
