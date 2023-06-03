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

InputSystem::InputSystem() : System({&typeid(PhysicalComponent), &typeid(ControllerComponent)}) {}
void InputSystem::ProcessInput(const Uint8* state) const {
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(ControllerComponent);
		component = (*entity)[cid];
		ControllerComponent* cc = static_cast<ControllerComponent*>(component);
		const tController *c = cc->Direction();
		
		int dx = 0;
		if (state[c->left]) dx -= cc->Speed();
		if (state[c->right]) dx += cc->Speed();
		
		int dy = 0;
		if (state[c->up]) dy -= cc->Speed();
		if (state[c->down]) dy += cc->Speed();
		
		cid = &typeid(PhysicalComponent);
		component = (*entity)[cid];
		PhysicalComponent* pc = static_cast<PhysicalComponent*>(component);
		
		b2Body *body = pc->Body();
		b2Vec2 v(dx, dy);
		body->SetLinearVelocity(v);
	}
};

void RenderSystem::Render() const {
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(PhysicalComponent);
		component = (*entity)[cid];
		PhysicalComponent* sc = static_cast<PhysicalComponent*>(component);
		
		b2Body* body = sc->Body();
		b2Fixture* fixture = body->GetFixtureList();
		b2Shape* shape = fixture->GetShape();
		b2Vec2 bodyPosition = body->GetPosition();
		float bodyAngle = body->GetAngle();
		
		if (shape->GetType() == b2Shape::e_polygon) {
			b2PolygonShape* polygonShape = static_cast<b2PolygonShape*>(shape);
			int vertexCount = polygonShape->m_count;
			
			// Assume the body's position and angle are in world coordinates
			b2Vec2 position = body->GetPosition();
			SDL_Log("body position at %0.0f,%0.0f", position.x, position.y);
			
			// Calculate the bounding box of the shape
			b2Vec2 minBounds = polygonShape->m_vertices[0];
			b2Vec2 maxBounds = polygonShape->m_vertices[0];
			for (int i = 1; i < vertexCount; ++i) {
				const b2Vec2& vertex = polygonShape->m_vertices[i];
				minBounds = b2Min(minBounds, vertex);
				maxBounds = b2Max(maxBounds, vertex);
			}
			
			// Convert the bounding box from Box2D coordinates to SDL_Rect
			SDL_Rect rect;
			rect.x = static_cast<int>(position.x + minBounds.x);
			rect.y = static_cast<int>(position.y + minBounds.y);
			rect.w = static_cast<int>(maxBounds.x - minBounds.x);
			rect.h = static_cast<int>(maxBounds.y - minBounds.y);
			
			SDL_RenderFillRect(this->renderer, &rect);
		}
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
CollisionSystem::CollisionSystem() : System({&typeid(PhysicalComponent)}) {}
bool CollisionSystem::Collides(const SDL_Rect& rect1, const SDL_Rect& rect2) const {
	SDL_Rect recrI;
	int result = SDL_IntersectRect(&rect1, &rect2, &recrI);
	/*
	if(result == SDL_TRUE) {
		SDL_Log("r1: %d,%d,%d,%d vs ...", rect1.x, rect1.y, rect1.w, rect1.h);
		SDL_Log("r2: %d,%d,%d,%d gives ...", rect2.x, rect2.y, rect2.w, rect2.h);
		SDL_Log("r3: %d,%d,%d,%d -> %d", recrI.x, recrI.y, recrI.w, recrI.h, result);
	}
	*/
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
			//if(collides)
			//	SDL_Log("- %s & %s ... %d", ent1->Name(), ent2->Name(), collides);
		}
	}
}

PhysicsSystem::PhysicsSystem(b2World &world) : System({&typeid(PhysicalComponent)}), world(world) {};
void PhysicsSystem::UpdateGame(float dt) {
	
	Component* component;
	const std::type_info* cid;
	for (Entity *entity: this->entities) {
		cid = &typeid(PhysicalComponent);
		component = (*entity)[cid];
		PhysicalComponent* pc = static_cast<PhysicalComponent*>(component);
		
		b2Body* body = pc->Body();

		b2Vec2 force(0.0f, 9.80665f);
		force *= body->GetMass();
		force *= dt * 100;
		body->ApplyForceToCenter(force, true);
	}
	int32 velocityIterations = 10;
	int32 positionIterations = 8;
	world.Step(dt, velocityIterations, positionIterations);
};
