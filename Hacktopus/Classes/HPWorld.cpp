//
//  HPWorld.cpp
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPWorld.h"

namespace HP
{
	World::World() :
		RN::World("GenericSceneManager")
	{
		_rng = new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister);
	}
	
	World::~World()
	{}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, (RN::Camera::Flags::Defaults | RN::Camera::Flags::Orthogonal) & ~RN::Camera::Flags::UseFog);
		RN::Vector2 resolution = RN::Window::GetSharedInstance()->GetSize();
		float aspect = resolution.y/resolution.x;
		float frustomHeight = 1920*aspect;
		_camera->SetOrthogonalFrustum(-frustomHeight*0.5f, frustomHeight*0.5f, -960.0f, 960.0f);
		_camera->SetClearColor(RN::Color::Black());
		
		RN::Billboard *background = new RN::Billboard();
		background->SetTexture(RN::Texture::WithFile("Textures/background.png"), 1.0f);
		background->SetPosition(RN::Vector3(0.0f, 0.0f, -100.0f));
		background->SetSize(RN::Vector2(1920.0f, 1200.0f));
		background->GetMaterial()->SetLighting(false);
		background->GetMaterial()->SetAmbientColor(RN::Color::White());
		background->GetMaterial()->SetBlending(false);
		
		Player::GetSharedInstance();
	}

	void World::FinishLoading(RN::Deserializer *deserializer)
	{
		_console = new HackingConsole();
		_console->Activate();
	}
	
	void World::Update(float delta)
	{
		_time -= delta;
		if(_time < 0.0f)
		{
			_time = _rng->GetRandomFloatRange(2.0f, 5.0f);
			Enemy *enemy = new Enemy();
			enemy->SetPosition(RN::Vector3(1000.0f*((_rng->GetRandomInt32Range(0.0f, 2.0f) >= 1.0f)?-1.0f:1.0f), _rng->GetRandomFloatRange(150.0f, 330.0f), -80.0f));
			if(enemy->GetPosition().x > 0.0f)
			{
				enemy->SetSize(enemy->GetSize()*RN::Vector2(-1.0f, 1.0f));
			}
			enemy->Release();
		}
	}
}
