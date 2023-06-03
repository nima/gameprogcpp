//
//  System.cpp
//  Game-mac
//
//  Created by Nima Talebi on 5/23/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#include "System.hpp"

#include <iostream>
#include <typeinfo>

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
		
		pc->UpdatePosition(sc, 0.0f, dy);
	}
};

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
void CollisionSystem::UpdateGame(float dt) {
	SDL_Rect rect1, rect2;
	for (auto ent1 : this->entities) {
		ent1->SetRect(rect1);

		for (auto ent2 : this->entities) {
			if(strcmp(ent1->Name(), ent2->Name()) >= 0) continue;
			
			ent2->SetRect(rect2);
			bool collides = this->Collides(static_cast<const SDL_Rect>(rect1), static_cast<const SDL_Rect>(rect2));
			if(collides)
				SDL_Log("- %s & %s ... %d", ent1->Name(), ent2->Name(), collides);
		}
	}
}

VelocitySystem::VelocitySystem() : System({&typeid(ShapeComponent), &typeid(PositionComponent), &typeid(VelocityComponent)}) {}
void VelocitySystem::Move(float dt) const {
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(VelocityComponent);
		component = (*entity)[cid];
		VelocityComponent* mc = static_cast<VelocityComponent*>(component);
		
		cid = &typeid(PositionComponent);
		component = (*entity)[cid];
		PositionComponent* pc = static_cast<PositionComponent*>(component);
		
		cid = &typeid(ShapeComponent);
		component = (*entity)[cid];
		ShapeComponent* sc = static_cast<ShapeComponent*>(component);
		
		float dxdt = mc->dxdt();
		float dydt = mc->dydt();
		pc->UpdatePosition(sc, dxdt*dt, dydt*dt);
	}
}

GravitySystem::GravitySystem(b2World &world) : System({&typeid(AccelerationComponent), &typeid(ShapeComponent), &typeid(PositionComponent)}), world(world) {};
void GravitySystem::UpdateGame(float dt) {
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(AccelerationComponent);
		component = (*entity)[cid];
		AccelerationComponent* gc = static_cast<AccelerationComponent*>(component);
		
		cid = &typeid(ShapeComponent);
		component = (*entity)[cid];
		ShapeComponent* sc = static_cast<ShapeComponent*>(component);
		
		cid = &typeid(PositionComponent);
		component = (*entity)[cid];
		PositionComponent* pc = static_cast<PositionComponent*>(component);

		/*
		b2Vec2 gravityForce = world.GetGravity();
		gravityForce *= gc->body->GetMass() * dt;
		gc->body->ApplyForceToCenter(gravityForce, true);
		
		// Move the entity based on the initial body position
		float deltaX = bodyDef.position.x - pc->x();
		float deltaY = bodyDef.position.y - pc->y();
		pc->Move(sc, deltaX, deltaY);
		
		int32 velocityIterations = 6; // Set the number of velocity iterations
		int32 positionIterations = 2; // Set the number of position iterations
		gc->Step(dt, velocityIterations, positionIterations);
		 */
	}
};
