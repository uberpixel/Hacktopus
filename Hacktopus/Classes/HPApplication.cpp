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
		
		RN::WindowConfiguration *configuration = new RN::WindowConfiguration(960/*640*/, /*360*/600);
		RN::Window::GetSharedInstance()->ActivateConfiguration(configuration->Autorelease(), 0);
		
		Intro *intro = new Intro();
		
		World *world = new World();
		RN::WorldCoordinator::GetSharedInstance()->LoadWorld(world->Autorelease());
		
		intro->Play([world]{
			world->LeftFromIntro();
		});
	}
	
	void Application::WillExit()
	{}
}
