//
//  System.cpp
//  Game-mac
//
//  Created by Nima Talebi on 5/23/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#include "System.hpp"

#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <cmath>

#include <SDL2/SDL.h>

System::System(const std::vector<const std::type_info*> responders) : responders(responders) {
	for (const auto responder : this->responders) {
		std::cout << "Responder: " << responder->name() << std::endl;
	}
};

// a registration means all responders (component types) must be attached to the entity
bool System::Register(Entity &entity) {
	Component* component;
	for (const auto responder : this->responders) {
		component = entity[responder];
		if (component == nullptr) return false;
	}
	this->entities.push_back(&entity);
	return true;
}

InputSystem::InputSystem() : System({&typeid(ShapeComponent), &typeid(PositionComponent), &typeid(ControllerComponent)}) {}
void InputSystem::ProcessInput(const Uint8* state) const {
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(ControllerComponent);
		component = (*entity)[cid];
		ControllerComponent* cc = static_cast<ControllerComponent*>(component);
		const tController *c = cc->Direction();
		
		/*
		 int dx = 0;
		 if (state[c->left]) dx -= cc->Speed();
		 if (state[c->right]) dx += mc->dxdt();
		 */
		
		int dy = 0;
		if (state[c->up]) dy -= cc->Speed();
		if (state[c->down]) dy += cc->Speed();
		
		cid = &typeid(ShapeComponent);
		component = (*entity)[cid];
		ShapeComponent* sc = static_cast<ShapeComponent*>(component);
		
		cid = &typeid(PositionComponent);
		component = (*entity)[cid];
		PositionComponent* pc = static_cast<PositionComponent*>(component);
		
		pc->Move(sc, 0.0f, dy);
	}
};
void InputSystem::UpdateGame(float dt) const {};
void InputSystem::GenerateOutput() const {};

VelocitySystem::VelocitySystem() : System({&typeid(ShapeComponent), &typeid(PositionComponent), &typeid(MobileComponent)}) {}
void VelocitySystem::UpdateGame(float dt) const {
	this->Move(dt);
}
void VelocitySystem::GenerateOutput() const {}
void VelocitySystem::ProcessInput(const Uint8 *state) const {}
void VelocitySystem::Move(float dt) const {
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(MobileComponent);
		component = (*entity)[cid];
		MobileComponent* mc = static_cast<MobileComponent*>(component);
		
		cid = &typeid(PositionComponent);
		component = (*entity)[cid];
		PositionComponent* pc = static_cast<PositionComponent*>(component);
		
		cid = &typeid(ShapeComponent);
		component = (*entity)[cid];
		ShapeComponent* sc = static_cast<ShapeComponent*>(component);

		float dxdt = mc->dxdt();
		float dydt = mc->dydt();
		pc->Move(sc, dxdt*dt, dydt*dt);
	}
}

RenderSystem::RenderSystem(SDL_Renderer* renderer) : System({&typeid(ShapeComponent), &typeid(PositionComponent)}) {
	this->renderer = renderer;
}
void RenderSystem::Render() const {
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(ShapeComponent);
		component = (*entity)[cid];
		ShapeComponent* sc = static_cast<ShapeComponent*>(component);
		
		cid = &typeid(PositionComponent);
		component = (*entity)[cid];
		PositionComponent* pc = static_cast<PositionComponent*>(component);
		
		SDL_Rect rect = {
			static_cast<int>(pc->x() - sc->w()/2.0f),
			static_cast<int>(pc->y() - sc->h()/2.0f),
			static_cast<int>(sc->w()),
			static_cast<int>(sc->h()),
		};
		SDL_RenderFillRect(this->renderer, &rect);
	}
}
void RenderSystem::UpdateGame(float dt) const {}
void RenderSystem::ProcessInput(const Uint8 *state) const {}
void RenderSystem::GenerateOutput() const {
	this->Render();
}


class MyContactListener : public b2ContactListener {
	void BeginContact(b2Contact* contact) {
		// React to the beginning of a collision
		// Access the collided bodies and perform your desired logic
	}
	
	void EndContact(b2Contact* contact) {
		// React to the end of a collision
	}
};


CollisionSystem::CollisionSystem() : System({&typeid(ShapeComponent), &typeid(PositionComponent)}) {}
void CollisionSystem::ProcessInput(const Uint8* state) const {}
bool CollisionSystem::Collides(const SDL_Rect& rect1, const SDL_Rect& rect2) const {
	SDL_Rect recrI;
	int result = SDL_IntersectRect(&rect1, &rect2, &recrI);
	if(result == SDL_TRUE) {
		SDL_Log("r1: %d,%d,%d,%d vs ...", rect1.x, rect1.y, rect1.w, rect1.h);
		SDL_Log("r2: %d,%d,%d,%d gives ...", rect2.x, rect2.y, rect2.w, rect2.h);
		SDL_Log("r3: %d,%d,%d,%d -> %d", recrI.x, recrI.y, recrI.w, recrI.h, result);
	}
	return (result == SDL_TRUE);
}
void CollisionSystem::UpdateGame(float dt) const {
	SDL_Rect rect1, rect2;
	for (auto ent1 : this->entities) {
		ent1->SetRect(rect1);

		for (auto ent2 : this->entities) {
			if(strcmp(ent1->Name(), ent2->Name()) <= 0) continue;
			
			ent2->SetRect(rect2);
			bool collides = this->Collides(static_cast<const SDL_Rect>(rect1), static_cast<const SDL_Rect>(rect2));
			if(collides)
				SDL_Log("- %s & %s ... %d", ent1->Name(), ent2->Name(), collides);
		}
	}
	/*
	// Bounce if needed
	for (const auto &paddle: paddles) {
		float diff = fabs(paddle.pos.y - ball.pos.y);
		float x1 = paddle.colXMin;
		float x2 = paddle.colXMax;
		if (x1 < 0) x1 += winWidth;
		if (x2 < 0) x2 += winWidth;
		
		if (diff >= paddleH / 2.0f) continue;
		
		const bool opIsLessThan = ball.pos.x < winWidth/2;
		if (ball.pos.x >= x1 && ball.pos.x <= x2 && (opIsLessThan ? (ball.vel.x < 0.0f) : (ball.vel.x > 0.0f))) {
			// Did we intersect with the paddle?
			// Our y-difference is small enough
			// We are in the correct x-position
			// The ball is moving to the left
			ball.vel.x *= -1.0f;
		} else if (ball.pos.x <= 0.0f || ball.pos.x >= winWidth) {
			// Did the ball go off the screen? (if so, end game)
			mIsRunning = false;
		}
	}
	
	if (ball.pos.y <= thickness && ball.vel.y < 0.0f) {
		// Did the ball collide with the top wall?
		ball.vel.y *= -1;
	} else if (ball.pos.y >= (winHeight - thickness) && ball.vel.y > 0.0f) {
		// Did the ball collide with the bottom wall?
		ball.vel.y *= -1;
	}*/
}
void CollisionSystem::GenerateOutput() const {}

GravitySystem::GravitySystem(b2World &world) : world(world), System({&typeid(ShapeComponent), &typeid(PositionComponent)}) {
	
};
void GravitySystem::UpdateGame(float dt) const {
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(ShapeComponent);
		component = (*entity)[cid];
		ShapeComponent* sc = static_cast<ShapeComponent*>(component);
		
		cid = &typeid(PositionComponent);
		component = (*entity)[cid];
		PositionComponent* pc = static_cast<PositionComponent*>(component);
		
		cid = &typeid(GravityComponent);
		component = (*entity)[cid];
		GravityComponent* gc = static_cast<GravityComponent*>(component);
	}
};
void GravitySystem::GenerateOutput() const {};
void GravitySystem::ProcessInput(const Uint8* state) const {};
