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
	{}
	
	World::~World()
	{}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, (RN::Camera::Flags::Defaults | RN::Camera::Flags::Orthogonal) & ~RN::Camera::Flags::UseFog);
		_camera->SetOrthogonalFrustum(0.0f, 1200.0f, 0.0f, 1920.0f);
		
		RN::Billboard *background = new RN::Billboard();
		background->SetTexture(RN::Texture::WithFile("Textures/background.png"), 1.0f);
		background->SetPosition(RN::Vector3(960.0f, 600.0f, -100.0f));
		background->SetSize(RN::Vector2(1920.0f, 1200.0f));
		background->GetMaterial()->SetLighting(false);
		background->GetMaterial()->SetAmbientColor(RN::Color::White());
		
		Enemy *enemy = new Enemy();
		enemy->SetPosition(RN::Vector3(200.0f, 500.0f, -80.0f));
	}

	void World::FinishLoading(RN::Deserializer *deserializer)
	{
		_console = new HackingConsole();
		_console->Activate();
	}
	
	void World::Update(float delta)
	{
		// Do something...
	}
}
