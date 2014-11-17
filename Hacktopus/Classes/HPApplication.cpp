//
//  HPApplication.cpp
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Nils Daumann, Sidney Just, Kerstin Buzelan und Norman v. Rechenberg
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
		
		RN::WindowConfiguration *configuration = (RN::WindowConfiguration *)RN::Window::GetSharedInstance()->GetMainScreen()->GetConfigurations()->GetLastObject();
		RN::Window::GetSharedInstance()->ActivateConfiguration(configuration->Autorelease(), RN::Window::Mask::Fullscreen);
		
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
