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
	: RN::Billboard(false)
	{
		SetTexture(RN::Texture::WithFile("Textures/enemy.png"));
		SetSize(RN::Vector2(GetTexture()->GetWidth(), GetTexture()->GetHeight()));
		GetMaterial()->SetLighting(false);
		GetMaterial()->SetDepthWrite(false);
		GetMaterial()->SetBlending(true);
		GetMaterial()->SetCullMode(RN::Material::CullMode::None);
	}
	
	Enemy::~Enemy()
	{
		
	}
	
	void Enemy::Update(float delta)
	{
		Translate(RN::Vector3(delta*200.0f*((GetPosition().x > 0)?-1.0f:1.0f), 0.0f, 0.0f));
		if(RN::Math::FastAbs(GetPosition().x) < 100)
		{
			RemoveFromWorld();
		}
	}
}