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
		
		_audioWorld = new RN::openal::AudioWorld();
		AddAttachment(_audioWorld);
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
		_camera->SetClipFar(20000);
		
		RN::openal::AudioListener *audioListener = new RN::openal::AudioListener();
		_camera->AddAttachment(audioListener);
		
		RN::Billboard *background = new RN::Billboard();
		background->SetTexture(RN::Texture::WithFile("Textures/background.png"), 1.0f);
		background->SetPosition(RN::Vector3(0.0f, 0.0f, -10000.0f));
		background->SetSize(RN::Vector2(1920.0f, 1200.0f));
		background->GetMaterial()->SetLighting(false);
		background->GetMaterial()->SetAmbientColor(RN::Color::White());
		background->GetMaterial()->SetBlending(false);
		
		RN::Camera *uiCamera = RN::UI::Server::GetSharedInstance()->GetCamera();
		uiCamera->SetFlags(uiCamera->GetFlags()|RN::Camera::Flags::NoFlush);
		RN::Texture *uiTexture = uiCamera->GetRenderTarget();
		RN::Billboard *uiBillboard = new RN::Billboard(false);
		uiBillboard->SetTexture(uiTexture, 1.0f);
		uiBillboard->SetPosition(RN::Vector3(0.0f, 0.0f, -9000));
		uiBillboard->SetScale(RN::Vector3(-1.0f, -1.0f, 1.0f));
		uiBillboard->GetMaterial()->SetLighting(false);
		uiBillboard->GetMaterial()->SetCullMode(RN::Material::CullMode::None);
		_camera->SetPriority(10000);
		
		Player::GetSharedInstance();
		ProgressDoor::GetSharedInstance();
	}

	void World::FinishLoading(RN::Deserializer *deserializer)
	{
		_console = new HackingConsole();
		_console->Activate();
	}
	
	void World::Screenshake(float time, float strength)
	{
		_shakeTime = time;
		_shakeStrength = strength;
	}
	
	void World::Update(float delta)
	{
		_time -= delta;
		if(_time < 0.0f)
		{
			_time = _rng->GetRandomFloatRange(2.0f, 5.0f);
			
			
			if(!_console->IsHacking())
				return;
			
			Enemy *enemy = new Enemy();
			enemy->SetPosition(RN::Vector3(1100.0f*((_rng->GetRandomInt32Range(0.0f, 2.0f) >= 1.0f)?-1.0f:1.0f), _rng->GetRandomFloatRange(150.0f, 330.0f), -8000.0f));
			if(enemy->GetPosition().x > 0.0f)
			{
				enemy->SetSize(enemy->GetSize()*RN::Vector2(-1.0f, 1.0f));
			}
			enemy->Release();
		}
		
		RN::Vector2 resolution = RN::Window::GetSharedInstance()->GetSize();
		float aspect = resolution.y/resolution.x;
		float frustomHeight = 1920*aspect;
		
		_shakeTime -= delta;
		if(_shakeTime > 0)
		{
			float shake = _rng->GetRandomFloatRange(1.0-_shakeStrength, 1.0f);
			_camera->SetOrthogonalFrustum(-frustomHeight*0.5f*shake, frustomHeight*0.5f*shake, -960.0f*shake, 960.0f*shake);
		}
		else
		{
			_camera->SetOrthogonalFrustum(-frustomHeight*0.5f, frustomHeight*0.5f, -960.0f, 960.0f);
		}
	}
}
