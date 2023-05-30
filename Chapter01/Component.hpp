//
//  Component.hpp
//  Game-mac
//
//  Created by Nima Talebi on 5/21/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#pragma once

#include <iostream>
#include <typeinfo>
#include <map>
#include <string>
#include <typeindex>

#include <SDL2/SDL_shape.h>
#include <box2d/box2d.h>

#include <stdio.h>

//typedef std::type_info ComponentId;
//typedef std::type_index ComponentIndex;

class Component {
public:
	Component(class Entity* owner);
	
	// all subclasses must declare themselves by type
	virtual const std::type_info* TypeId() const = 0;
	
	//virtual ~Component();
	//virtual void Update(float dt);
	
protected:
	class Entity *owner;
};

struct tController { int up; int down; int left; int right; };
class ControllerComponent : public Component {
public:
	ControllerComponent(Entity *owner, float speed, tController controller);
	const std::type_info* TypeId() const override;
	const tController* Direction() const;
	float Speed() const;
private:
	tController controller;
	float speed;
};

struct tDimensions { float width; float height; };

class ShapeComponent : public Component {
public:
	ShapeComponent(Entity* owner, tDimensions dimensions);
	const std::type_info* TypeId() const override {
		return &typeid(ShapeComponent);
	}
	float w() const { return this->dimensions.width; }
	float h() const { return this->dimensions.height; }
private:
	tDimensions dimensions;
};

struct tPosition { float x; float y; };
struct tBoundary { int x0; int y0; int xN; int yN; };
class PositionComponent : public Component {
public:
	PositionComponent(Entity *owner, tPosition position, tBoundary boundary);
	const std::type_info* TypeId() const override;
	void Move(ShapeComponent *sc, float dx, float dy);
	float x() const;
	float y() const;
private:
	tPosition position;
	tBoundary boundary;
};

class PhysicsComponent : public Component {
public:
	PhysicsComponent(Entity *owner);
	const std::type_info* TypeId() const override;
private:
};

class GravityComponent : public Component {
public:
	GravityComponent(Entity *owner, b2World &world);
	const std::type_info* TypeId() const override;
private:
	b2World &world;
};

struct tVelocity { float dxdt; float dydt; };
class MobileComponent : public Component {
public:
	MobileComponent(Entity *owner, tVelocity velocity);
	const std::type_info* TypeId() const override;
	float dxdt() const;
	float dydt() const;
private:
	tVelocity velocity;
};

