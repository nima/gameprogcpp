//
//  Registry.cpp
//  Game-mac
//
//  Created by Nima Talebi on 5/23/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//

#include "Registry.hpp"

Registry::Registry() {}

Registry::~Registry() {}

void Registry::Register(System *system) {
	this->systems.push_back(system);
}

void Registry::Register(Entity *entity) {
	for (System *system: this->systems) {
		system->Register(*entity);
	}
}

void Registry::ProcessInput(const Uint8* state) {
	for (auto system : this->systems) {
		system->ProcessInput(state);
	}
}

void Registry::UpdateGame(float dt) {
	for (auto system : this->systems) {
		system->UpdateGame(dt);
	}
}

void Registry::GenerateOutput() {
	for (auto system : this->systems) {
		system->GenerateOutput();
	}
}
