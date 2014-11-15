//
//  HPPlayer.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPPlayer.h"

namespace HP
{
	RNDefineSingleton(Player)
	
	Player::Player()
	{
		_body = new RN::Billboard(RN::Texture::WithFile("Textures/hektor.png"));
		_body->SetPosition(RN::Vector3(-70.0f, 280.0f, -20.0f));
		_body->GetMaterial()->SetLighting(false);
		_body->GetMaterial()->SetAmbientColor(RN::Color::White());
		_body->GetMaterial()->SetDepthWrite(false);
		
		_armRight = new AnimatableEntity();
		_armRight->SetDefaultTexture(RN::Texture::WithFile("Textures/tentakelarm+0.png"));
		_armRight->SetPosition(RN::Vector3(430.0f, 280.0f, -20.0f));
		_armRight->GetMaterial()->SetLighting(false);
		_armRight->GetMaterial()->SetAmbientColor(RN::Color::White());
		_armRight->GetMaterial()->SetDepthWrite(false);
		
		_armLeft = new AnimatableEntity(false);
		_armLeft->SetDefaultTexture(RN::Texture::WithFile("Textures/tentakelarm+0.png"));
		_armLeft->SetPosition(RN::Vector3(-540.0f, 290.0f, -20.0f));
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
		RN::Vector3 position = target->GetPosition();
		target->kill();
	
		if(position.x > 0)
			_armRight->PlayAnimation("Textures/tentakelarm");
		else
			_armLeft->PlayAnimation("Textures/tentakelarm");
	}
}