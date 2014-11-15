//
//  HPEnemy.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 14.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPEnemy.h"
#include "HPPlayer.h"

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
		
		RN::Input *input = RN::Input::GetSharedInstance();
		
		float xblubb = RN::Math::FastAbs(input->GetMousePosition().x*2.0f-960-GetPosition().x);
		float yblubb = RN::Math::FastAbs(input->GetMousePosition().y*2.0f-600-GetPosition().y);
		if(xblubb < RN::Math::FastAbs(GetSize().x*0.5f))
		{
			if(yblubb < RN::Math::FastAbs(GetSize().y*0.5f))
			{
				if(input->IsMousePressed(0))
				{
					Player::GetSharedInstance()->Attack(this);
				}
			}
		}
	}
}