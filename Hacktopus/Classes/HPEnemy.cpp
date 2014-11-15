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
	
	Enemy::Enemy() :
		AnimatableEntity(false),
		_dead(false),
		_active(true),
		_qteItems(new RN::Array())
	{
		SetDefaultTexture(RN::Texture::WithFile("Textures/Mensch1_walk+0.png"));
		SetSize(RN::Vector2(GetTexture()->GetWidth(), GetTexture()->GetHeight())*0.8f);
		GetMaterial()->SetLighting(false);
		GetMaterial()->SetDepthWrite(false);
		GetMaterial()->SetBlending(true);
		GetMaterial()->SetCullMode(RN::Material::CullMode::None);
		
		PlayAnimation("Textures/Mensch1_walk");
		RepeateAnimation();
	}
	
	Enemy::~Enemy()
	{
		_qteItems->Release();
	}
	
	RN::Texture *Enemy::GetTextureForButton(int button)
	{
		switch(button)
		{
			case 4:
				return RN::Texture::WithFile("Textures/PS4/PS4_Triangle.png");
			case 5:
				return RN::Texture::WithFile("Textures/PS4/PS4_Circle.png");
			case 6:
				return RN::Texture::WithFile("Textures/PS4/PS4_Cross.png");
			case 7:
				return RN::Texture::WithFile("Textures/PS4/PS4_Square.png");
			default:
				return nullptr;
		}
	}
	
	
	bool Enemy::ConsumeButton(int button)
	{
		if(_qte.empty())
			return false;
		
		if(button == _qte.front())
		{
			_qte.erase(_qte.begin());
			
			UpdateQTEItems();
			
			if(_qte.size() == 0)
			{
				Player::GetSharedInstance()->Attack(this);
				_active = false;
			}
			
			return true;
		}
		
		return false;
	}
	
	void Enemy::UpdateQTEItems()
	{
		_qteItems->Enumerate<RN::SceneNode>([](RN::SceneNode *node, size_t index, bool &stop) {
			node->RemoveFromParent();
		});
		_qteItems->RemoveAllObjects();
		
		float offset = 0;
		
		for(int button : _qte)
		{
			RN::Texture *texture = GetTextureForButton(button);
			RN::Billboard *billboard = new RN::Billboard();
			billboard->SetTexture(texture);
			billboard->SetPosition(RN::Vector3(offset, -200, 0));
			billboard->SetSize(RN::Vector2(64, 64));
			billboard->GetMaterial()->SetLighting(false);
			
			offset += 70;
			
			AddChild(billboard);
			_qteItems->AddObject(billboard->Autorelease());
		}
	}
	
	void Enemy::GenerateQTE()
	{
		std::vector<int> buttons{4, 5, 6, 7};
		
		RN::Random::MersenneTwister random;
		
		for(int i = 0; i < 3; i ++)
			_qte.push_back(buttons.at(random.GetRandomInt32Range(0, static_cast<int32>(buttons.size()))));
		
		UpdateQTEItems();
	}
	
	void Enemy::Update(float delta)
	{
		AnimatableEntity::Update(delta);
		
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
				_active = false;
			}
		}
		else
		{
			Rotate(RN::Vector3(0.0f, 0.0f, 300.0f*delta*((GetPosition().x > 0)?1.0f:-1.0f)));
			Translate(RN::Vector3(delta*1000.0f*((GetPosition().x > 0)?1.0f:-1.0f), -500.0f*delta, 0.0f));
			if(RN::Math::FastAbs(GetPosition().x) > 1100)
			{
				RemoveFromWorld();
				_active = false;
			}
		}
	}
	
	void Enemy::kill()
	{
		_dead = true;
		_active = false;
	}
}