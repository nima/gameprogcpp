// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Actor.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor *owner, int drawOrder)
	: SpriteComponent(owner, drawOrder), mCurrFrame(0.0f), mAnimFPS(24.0f) {
}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAssets[mCurrFrameSet].textures.size() > 0) {
		if ((mCurrFrameSet == 0) || this->mOwner->IsMoving()) {
			// Update the current frame based on frame rate
			// and delta time
			mCurrFrame += mAnimFPS * deltaTime / 2;

			// Wrap current frame if needed
			while (mCurrFrame >= mAssets[mCurrFrameSet].textures.size()) {
				mCurrFrame -= mAssets[mCurrFrameSet].textures.size();
			}
		}

		// Set the current texture
		SetTexture(mAssets[mCurrFrameSet].textures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAssets(const std::initializer_list<tAsset> assets) {
	for (auto asset : assets)
		mAssets.push_back(asset);

	if (mAssets[mCurrFrameSet].textures.size() > 0) {
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAssets[mCurrFrameSet].textures[0]);
	}
}
