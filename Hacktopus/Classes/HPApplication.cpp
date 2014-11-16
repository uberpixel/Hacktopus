//
//  HPApplication.cpp
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPApplication.h"

namespace HP
{
	Application::Application()
	{}
	Application::~Application()
	{}
	
	
	void Application::Start()
	{
		SetTitle("Hacktopus");
		
		RN::WindowConfiguration *configuration = new RN::WindowConfiguration(1280, 720);
		RN::Window::GetSharedInstance()->ActivateConfiguration(configuration->Autorelease(), 0);
		
		Intro *intro = new Intro();
		
		_world = new World();
		
		RN::WorldCoordinator::GetSharedInstance()->LoadWorld(_world->Autorelease());
		
		intro->Play([=] {
			_world->LeftFromIntro();
		});
	}
	
	void Application::RecreateWorld()
	{
		_world->Reset();
	}
	
	void Application::WillExit()
	{}
}
