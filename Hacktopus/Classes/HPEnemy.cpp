//
//  HPEnemy.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 14.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPEnemy.h"

namespace HP
{
	RNDefineMeta(Enemy, RN::Billboard)
	
	Enemy::Enemy()
	: RN::Billboard()
	{
		SetTexture(RN::Texture::WithFile("Textures/enemy.png"));
		SetSize(RN::Vector2(GetTexture()->GetWidth(), GetTexture()->GetHeight()));
		GetMaterial()->SetLighting(false);
	}
	
	Enemy::~Enemy()
	{
		
	}
	
	void Enemy::Update(float delta)
	{
		
	}
}