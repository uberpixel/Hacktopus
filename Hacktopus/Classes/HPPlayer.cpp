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
	
	Player::Player()
	{
		_body = new RN::Billboard(RN::Texture::WithFile("Textures/hektor.png"));
		_body->SetPosition(RN::Vector3(0.0f, 300.0f, -4000.0f));
		_body->GetMaterial()->SetLighting(false);
		_body->GetMaterial()->SetAmbientColor(RN::Color::White());
		_body->GetMaterial()->SetDepthWrite(false);
		
		_armRight = new AnimatableEntity();
		_armRight->SetDefaultTexture(RN::Texture::WithFile("Textures/tentakelarm+0.png"));
		_armRight->SetPosition(RN::Vector3(500.0f, 290.0f, -2000.0f));
		_armRight->GetMaterial()->SetLighting(false);
		_armRight->GetMaterial()->SetAmbientColor(RN::Color::White());
		_armRight->GetMaterial()->SetDepthWrite(false);
		
		_armLeft = new AnimatableEntity(false);
		_armLeft->SetDefaultTexture(RN::Texture::WithFile("Textures/tentakelarm+0.png"));
		_armLeft->SetPosition(RN::Vector3(-470.0f, 310.0f, -2000.0f));
		_armLeft->GetMaterial()->SetLighting(false);
		_armLeft->GetMaterial()->SetAmbientColor(RN::Color::White());
		_armLeft->GetMaterial()->SetDepthWrite(false);
		_armLeft->GetMaterial()->SetCullMode(RN::Material::CullMode::None);
	}
	
	Player::~Player()
	{
		
	}
	
	void Player::Attack(Enemy *target)
	{
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