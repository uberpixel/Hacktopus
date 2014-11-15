//
//  HPIntro.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPIntro.h"

namespace HP
{
	Intro::Intro() : RN::UI::Widget(RN::UI::Widget::Style::Borderless), _state(-1)
	{
		SetFrame(RN::Rect(0, 0, RN::Window::GetSharedInstance()->GetSize().x, RN::Window::GetSharedInstance()->GetSize().y));
		GetContentView()->SetBackgroundColor(RN::UI::Color::Black());
		
	}
	
	Intro::~Intro()
	{
		
	}
	
	void Intro::Play(RN::Function &&f)
	{
		_callback = std::move(f);
		StepForward();
	}
	
	void Intro::StepForward()
	{
		_state += 1;
		
		switch(_state)
		{
			case 0:
				RN::Timer::ScheduledTimerWithDuration(std::chrono::seconds(3), [this]{StepForward();}, false);
				return;
			case 1:
				RN::Timer::ScheduledTimerWithDuration(std::chrono::seconds(3), [this]{StepForward();}, false);
				return;
			case 2:
				RN::Timer::ScheduledTimerWithDuration(std::chrono::seconds(3), [this]{StepForward();}, false);
				return;
			default:
				_callback();
				return;
		}
	}
}
