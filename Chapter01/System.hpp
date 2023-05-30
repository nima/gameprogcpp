//
//  System.hpp
//  Chapter01-mac
//
//  Created by Nima Talebi on 5/23/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#pragma once

#include <map>
#include <vector>
#include <string>
#include <typeinfo>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "Entity.hpp"
#include "Component.hpp"

class System {
public:
	System(const std::vector<const std::type_info*> responders);
	bool Register(Entity &entity);
	
	virtual void ProcessInput(const Uint8* state) const = 0;
	virtual void UpdateGame(float dt) const = 0;
	virtual void GenerateOutput() const = 0;
protected:
	std::vector<Entity*> entities;
private:
	const std::vector<const std::type_info*> responders;
};

class InputSystem : public System {
public:
	InputSystem();
	void ProcessInput(const Uint8* state) const override;
	void UpdateGame(float dt) const override;
	void GenerateOutput() const override;
};

class VelocitySystem : public System {
public:
	VelocitySystem();
	void ProcessInput(const Uint8* state) const override;
	void UpdateGame(float dt) const override;
	void GenerateOutput() const override;
private:
	void Move(float dt) const;
};

class CollisionSystem : public System {
public:
	CollisionSystem();
	void ProcessInput(const Uint8* state) const override;
	void UpdateGame(float dt) const override;
	void GenerateOutput() const override;
	bool Collides(const SDL_Rect& rect1, const SDL_Rect& rect2) const;
};

class GravitySystem : public System {
public:
	GravitySystem(b2World& world);
	void ProcessInput(const Uint8* state) const override;
	void UpdateGame(float dt) const override;
	void GenerateOutput() const override;
private:
	b2World& world;
};

class RenderSystem : public System {
public:
	RenderSystem(SDL_Renderer* renderer);
	void ProcessInput(const Uint8* state) const override;
	void UpdateGame(float dt) const override;
	void GenerateOutput() const override;
private:
	void Render() const;
	SDL_Renderer *renderer;
};
