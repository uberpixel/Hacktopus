//
//  HPOutro.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPOutro.h"
#include "HPWorld.h"

namespace HP
{
	Outro::Outro() :
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
	
	Outro::~Outro()
	{}
	
	void Outro::Play(RN::Function &&f, bool win)
	{
		_callback = std::move(f);
		World::GetActiveWorld()->Downcast<World>()->PlayMusic(0);
		
		if(!win)
		{
			_state = 100;
			
			World::GetActiveWorld()->Downcast<World>()->PlayMusic(4);
		}
		else
		{
			if(RN::Random::MersenneTwister().GetRandomInt32Range(0, 1))
			{
				_state = 10;
			}
		}
		
		StepForward();
		Open();
	}
	
	RN::Texture *Outro::GetOutroImageWithID(size_t index)
	{
		std::stringstream stream;
		stream << "Outro/outro_frame" << index << ".png";
		
		return RN::Texture::WithFile(stream.str());
	}
	
	void Outro::StepForward()
	{
		_state ++;
		
		if(_state == 2 || _state == 12 || _state == 102)
		{
			RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this](RN::Message *message) {
				
				RN::Kernel::GetSharedInstance()->ScheduleFunction([this] {
					
					_callback();
					
					Close();
					RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
					
				});
			}, this);
			
			return;
		}
		
		RN::UI::Image *image = new RN::UI::Image(GetOutroImageWithID(_state));
		_imageView->SetImage(image->Autorelease());
		
		uint32_t wait = 3000;
		RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(wait), [this]{ StepForward(); }, false);
	}
}
