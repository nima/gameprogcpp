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
const std::type_info* ShapeComponent::TypeId() const {
	return &typeid(ShapeComponent);
}
float ShapeComponent::w() const {
	return this->dimensions.width;
}
float ShapeComponent::h() const {
	return this->dimensions.height;
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

PhysicsComponent::PhysicsComponent(Entity *owner, tCollisionZoneX czx) : Component(owner) {
	this->czx = czx;
}
const std::type_info* PhysicsComponent::TypeId() const {
	return &typeid(PhysicsComponent);
}
