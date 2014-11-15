//
//  HPEnemy.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 14.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPEnemy.h"
#include "HPPlayer.h"
#include "HPProgressDoor.h"
#include "HPWorld.h"

namespace HP
{
	RNDefineMeta(Enemy, RN::Billboard)
	
	Enemy::Enemy()
	: RN::Billboard(false), _dead(false)
	{
		SetTexture(RN::Texture::WithFile("Textures/enemy.png"));
		SetSize(RN::Vector2(GetTexture()->GetWidth(), GetTexture()->GetHeight())*0.8f);
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
		if(!_dead)
		{
			Translate(RN::Vector3(delta*200.0f*((GetPosition().x > 0)?-1.0f:1.0f), 0.0f, 0.0f));
		
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
			
			if(RN::Math::FastAbs(GetPosition().x) < 100)
			{
				ProgressDoor::GetSharedInstance()->Progress(-5.0f);
				World::GetActiveWorld()->Downcast<World>()->Screenshake();
				RemoveFromWorld();
			}
		}
		else
		{
			Rotate(RN::Vector3(0.0f, 0.0f, 300.0f*delta*((GetPosition().x > 0)?1.0f:-1.0f)));
			Translate(RN::Vector3(delta*1000.0f*((GetPosition().x > 0)?1.0f:-1.0f), -500.0f*delta, 0.0f));
			if(RN::Math::FastAbs(GetPosition().x) > 1100)
			{
				RemoveFromWorld();
			}
		}
	}
	
	void Enemy::kill()
	{
		_dead = true;
	}
}