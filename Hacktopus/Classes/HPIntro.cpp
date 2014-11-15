//
//  HPIntro.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPIntro.h"
#include "HPWorld.h"

uint32 times[13] = {
	500, 500, 1500, 1500,
	1500, 1500, 1500,
	2000, 1500, 2500,
	1000, 1500, 3800
};

namespace HP
{
	Intro::Intro() :
		RN::UI::Widget(RN::UI::Widget::Style::Borderless),
		_state(0)
	{
		SetFrame(RN::Rect(0, 0, RN::Window::GetSharedInstance()->GetSize().x, RN::Window::GetSharedInstance()->GetSize().y));
		GetContentView()->SetBackgroundColor(RN::UI::Color::Black());
		
		_imageView = new RN::UI::ImageView();
		_imageView->SetFrame(GetFrame());
		_imageView->SetAutoresizingMask(RN::UI::View::AutoresizingMask::FlexibleWidth | RN::UI::View::AutoresizingMask::FlexibleHeight);
		
		GetContentView()->AddSubview(_imageView);
	}
	
	Intro::~Intro()
	{}
	
	void Intro::Play(RN::Function &&f)
	{
		_callback = std::move(f);
		_stopped = false;
		
		StepForward();
		Open();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this](RN::Message *message) {
			
			RN::Kernel::GetSharedInstance()->ScheduleFunction([this] {
				
				_callback();
				_stopped = true;
				
				Close();
				RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
				
			});
		}, this);
	}
	
	RN::Texture *Intro::GetIntroImageWithID(size_t index)
	{
		std::stringstream stream;
		stream << "Intro/intro_frame" << index << ".png";
		
		return RN::Texture::WithFile(stream.str());
	}
	
	void Intro::StepForward()
	{
		if(_stopped)
			return;
		
		_state ++;
		
		if(_state >= 14)
		{
			Close();
			_callback();
			
			RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
			return;
		}
		
		RN::UI::Image *image = new RN::UI::Image(GetIntroImageWithID(_state));
		_imageView->SetImage(image->Autorelease());
		
		RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(times[_state - 1]), [this]{ StepForward(); }, false);
		
		if(_state == 13)
		{
			RN::AudioResource *audio = RN::AudioResource::WithFile("Intro/DUNDUNDUUUUUN.ogg");
			World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
		}
	}
}
