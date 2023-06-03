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

struct tPosition { float x; float y; };
struct tBoundary { int x0; int y0; int xN; int yN; };
class PhysicalComponent : public Component {
public:
	PhysicalComponent(Entity *owner, b2World &world, b2Body *body, tBoundary boundary);
	~PhysicalComponent();
	const std::type_info* TypeId() const override { return &typeid(PhysicalComponent); }
	void Update(float dt) override {};
	b2Body *Body() { return this->body; }
private:
	b2World &world;
	b2Body *body;
	tBoundary boundary;
};
