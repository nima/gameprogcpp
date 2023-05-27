// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------
#pragma once

#include <initializer_list>
#include <vector>

#include "SpriteComponent.h"

struct tAsset {
	std::vector<SDL_Texture *> textures;
	float speed;
	bool loop;
	bool inactiveWithoutKeyPress;
};

class AnimSpriteComponent : public SpriteComponent {
  public:
	AnimSpriteComponent(class Actor *owner, int drawOrder = 100);
	// Update animation every frame (overridden from component)
	void Update(float deltaTime) override;
	// Set the textures used for animation
	void SetAssets(const std::initializer_list<tAsset> assets);
	// Set/get the animation FPS
	float GetAnimFPS() const {
		return mAnimFPS;
	}
	void SetAnimFPS(float fps) {
		mAnimFPS = fps;
	}

	// Current frame set
	float mCurrFrameSet;

  private:
	// All textures in the animation
	std::vector<tAsset> mAssets;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
};
