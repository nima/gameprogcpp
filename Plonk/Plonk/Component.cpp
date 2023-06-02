//
//  Component.cpp
//  Game-mac
//
//  Created by Nima Talebi on 5/21/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#include "Component.hpp"

Component::Component(class Entity* owner) {
	this->owner = owner;
}
//virtual ~Component();
//virtual void Update(float dt);

ControllerComponent::ControllerComponent(Entity *owner, float speed, tController controller) : Component(owner) {
	this->speed = speed;
	this->controller = controller;
}
float ControllerComponent::Speed() const {
	return this->speed;
}
const std::type_info* ControllerComponent::TypeId() const {
	return &typeid(ControllerComponent);
}
const tController* ControllerComponent::Direction() const {
	return &this->controller;
}

ShapeComponent::ShapeComponent(Entity *owner, tDimensions dimensions) : Component(owner) {
	this->dimensions = dimensions;
}

PositionComponent::PositionComponent(Entity *owner, tPosition position, tBoundary boundary) : Component(owner) {
	this->position = position;
	this->boundary = boundary;
}
const std::type_info* PositionComponent::TypeId() const {
	return &typeid(PositionComponent);
}
void PositionComponent::Move(ShapeComponent *sc, float dx, float dy) {
	this->position.y += (dy);
	if (this->position.y < (sc->h()/2.0f + sc->w())) {
		this->position.y = sc->h()/2.0f + sc->w();
	} else if (this->position.y > (this->boundary.yN - sc->h()/2.0f)) {
		this->position.y = this->boundary.yN - sc->h()/2.0f;
	}

	this->position.x += dx;
}
float PositionComponent::x() const {
	return this->position.x;
}
float PositionComponent::y() const {
	return this->position.y;
}

MobileComponent::MobileComponent(Entity *owner, tVelocity velocity) : Component(owner) {
	this->velocity = velocity;
}
const std::type_info* MobileComponent::TypeId() const {
	return &typeid(MobileComponent);
}
float MobileComponent::dxdt() const {
	return this->velocity.dxdt;
}
float MobileComponent::dydt() const {
	return this->velocity.dydt;
}

PhysicsComponent::PhysicsComponent(Entity *owner) : Component(owner) {
}
const std::type_info* PhysicsComponent::TypeId() const {
	return &typeid(PhysicsComponent);
}

GravityComponent::GravityComponent(Entity *owner, b2World &world) : Component(owner), world(world) {
	// Create a Box2D body for the entity
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; // The body will move in response to forces
	bodyDef.position.Set(0.0f, 0.0f); // Set the initial position of the body
	b2Body* body = world.CreateBody(&bodyDef);
}
const std::type_info* GravityComponent::TypeId() const {
	return &typeid(GravityComponent);
}
