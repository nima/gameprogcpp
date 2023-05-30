//
//  TileMapComponent.h
//  Game-mac
//
//  Created by Nima Talebi on 5/27/23.
//  Copyright © 2023 Sanjay Madhav. All rights reserved.
//
#pragma once

#include <string>

#include "Math.h"

#include "SpriteComponent.h"
#include "Actor.h"

class TileMapComponent : public SpriteComponent {
public:
	TileMapComponent(class Actor *owner, std::string tilemapFilename, int drawOrder = 9);
	
	// Update animation every frame (overridden from component)
	void Update(float deltaTime) override;
	virtual void Draw(SDL_Renderer *renderer) override;
	
	// Set the textures used for the background
	void SetBGTextures(const std::vector<SDL_Texture *> &textures);
	// Get/set screen size and scroll speed
	void SetScreenSize(const Vector2 &size) {
		mScreenSize = size;
	}
	void SetScrollSpeed(float speed) {
		mScrollSpeed = speed;
	}
	float GetScrollSpeed() const {
		return mScrollSpeed;
	}
private:
	std::string mTileMapFilename;
	
	// Struct to encapsulate each bg image and its offset
	struct BGTexture {
		SDL_Texture *mTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};
