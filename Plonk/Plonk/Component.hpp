//
//  Component.hpp
//  Game-mac
//
//  Created by Nima Talebi on 5/21/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#pragma once

#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>

#include <SDL2/SDL_shape.h>
#include <box2d/box2d.h>

//typedef std::type_info ComponentId;
//typedef std::type_index ComponentIndex;

class Component {
public:
	Component(class Entity* owner);
	virtual ~Component() = default;

	// all subclasses must declare themselves by type
	virtual const std::type_info* TypeId() const = 0;
	virtual void Update(float dt) = 0;
protected:
	class Entity *owner;
};

struct tController { int up; int down; int left; int right; };
class ControllerComponent : public Component {
public:
	ControllerComponent(Entity *owner, float speed, tController controller);
	const std::type_info* TypeId() const override { return &typeid(ControllerComponent); }
	void Update(float dt) override {};
	float Speed() const { return this->speed; }
	const tController* Direction() const { return &this->controller; }
private:
	tController controller;
	float speed;
};

struct tDimensions { float width; float height; };
class ShapeComponent : public Component {
public:
	ShapeComponent(Entity* owner, tDimensions dimensions);
	const std::type_info* TypeId() const override { return &typeid(ShapeComponent); }
	void Update(float dt) override {};
	float w() const { return this->dimensions.width; }
	float h() const { return this->dimensions.height; }
private:
	tDimensions dimensions;
};

struct tPosition { float x; float y; };
struct tBoundary { int x0; int y0; int xN; int yN; };
class PositionComponent : public Component {
public:
	PositionComponent(Entity *owner, b2World &world, tPosition position, tBoundary boundary);
	const std::type_info* TypeId() const override { return &typeid(PositionComponent); }
	void Update(float dt) override {};
	void UpdatePosition(ShapeComponent *sc, float dx, float dy);
	float x() const { return this->position.x; }
	float y() const { return this->position.y; }
private:
	b2World &world;
	b2Body* body;
	tPosition position;
	tBoundary boundary;
};

struct tVelocity { float dxdt; float dydt; };
class VelocityComponent : public Component {
public:
	VelocityComponent(Entity *owner, b2World &world, tVelocity velocity);
	const std::type_info* TypeId() const override { return &typeid(VelocityComponent); }
	void Update(float dt) override {};
	float dxdt() const { return this->velocity.dxdt; }
	float dydt() const { return this->velocity.dydt; }
private:
	b2World &world;
	tVelocity velocity;
};

class AccelerationComponent : public Component {
public:
	AccelerationComponent(Entity *owner, b2World &world);
	void Update(float dt) override {
		int32 velocityIterations = 6;
		int32 positionIterations = 2;
		this->world.Step(dt, velocityIterations, positionIterations);
	}
	const std::type_info* TypeId() const override { return &typeid(AccelerationComponent); }
private:
	b2World &world;
};

