//
//  HPPlayer.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPPlayer.h"
#include "HPWorld.h"

namespace HP
{
	RNDefineSingleton(Player)
	
	Player::Player() : _numberOfLifes(4)
	{
		_body = new AnimatableEntity();
		_body->SetDefaultTexture(RN::Texture::WithFile("Textures/hektor2.png"));
		_body->SetPosition(RN::Vector3(0.0f, 300.0f, -4000.0f));
		_body->GetMaterial()->SetLighting(false);
		_body->GetMaterial()->SetAmbientColor(RN::Color::White());
		_body->GetMaterial()->SetDepthWrite(false);
		
		_armRight = new AnimatableEntity();
		_armRight->SetDefaultTexture(RN::Texture::WithFile("Textures/tentakelarm+0.png"));
		_armRight->SetPosition(RN::Vector3(500.0f, 290.0f, -3000.0f));
		_armRight->GetMaterial()->SetLighting(false);
		_armRight->GetMaterial()->SetAmbientColor(RN::Color::White());
		_armRight->GetMaterial()->SetDepthWrite(false);
		
		_armLeft = new AnimatableEntity(false);
		_armLeft->SetDefaultTexture(RN::Texture::WithFile("Textures/tentakelarm+0.png"));
		_armLeft->SetPosition(RN::Vector3(-470.0f, 310.0f, -3000.0f));
		_armLeft->GetMaterial()->SetLighting(false);
		_armLeft->GetMaterial()->SetAmbientColor(RN::Color::White());
		_armLeft->GetMaterial()->SetDepthWrite(false);
		_armLeft->GetMaterial()->SetCullMode(RN::Material::CullMode::None);
		
		for(int i = 0; i < 5; i++)
		{
			_lifes[i] = new RN::Billboard(RN::Texture::WithFile("Textures/health_button_green_50x50.png"));
			_lifes[i]->GetMaterial()->SetLighting(false);
		}
		
		_lifes[0]->SetPosition(RN::Vector3(-893.0f, 202.0f, -8000.0f));
		_lifes[1]->SetPosition(RN::Vector3(-815.0f, 202.0f, -8000.0f));
		_lifes[2]->SetPosition(RN::Vector3(-737.0f, 202.0f, -8000.0f));
		_lifes[3]->SetPosition(RN::Vector3(-655.0f, 202.0f, -8000.0f));
		_lifes[4]->SetPosition(RN::Vector3(-575.0f, 202.0f, -8000.0f));
	}
	
	Player::~Player()
	{
		
	}
	
	void Player::Reset()
	{
		_numberOfLifes = 4;
		
		for(int i = 0; i < 5; i++)
		{
			_lifes[i]->SetTexture(RN::Texture::WithFile("Textures/health_button_green_50x50.png"));
		}
	}
	
	void Player::RemoveLife()
	{
		if(_numberOfLifes < 0)
		{
			World::GetActiveWorld()->Downcast<World>()->PlayOutro(false);
			World::GetActiveWorld()->Downcast<World>()->GameEnded();
			
			return;
		}
		
		_lifes[_numberOfLifes --]->SetTexture(RN::Texture::WithFile("Textures/health_button_red_50x50.png"));
	}
	
	void Player::KeepTyping()
	{
		if(!_body->IsPlaying())
			_body->PlayAnimationFile("Animations/hektor_type1.json");
	}
	
	void Player::Attack(Enemy *target)
	{
		if(target->IsKilled())
			return;
		
		RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/test.ogg");
		World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
		
		RN::Vector3 position = target->GetPosition();
		target->kill();
	
		if(position.x > 0)
			_armRight->PlayAnimationFile("Animations/hektor_slap1.json");
		else
			_armLeft->PlayAnimationFile("Animations/hektor_slap1.json");
	}
}