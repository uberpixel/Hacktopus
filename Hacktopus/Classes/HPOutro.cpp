//
//  HPOutro.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPOutro.h"
#include "HPWorld.h"

uint32 times_win[3] = {
	1000, 1000, 1000
};

uint32 times_lose[1] = {
	6000
};

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
		
		if(!win)
		{
			_state = 100;
			
			RN::AudioResource *audio = RN::AudioResource::WithFile("Outro/ende.ogg");
			World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
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
		
		if(_state == 3 || _state == 102)
		{
			Close();
			_callback();
			
			RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
			return;
		}
		
		RN::UI::Image *image = new RN::UI::Image(GetOutroImageWithID(_state));
		_imageView->SetImage(image->Autorelease());
		
		uint32_t wait = (_state >= 100) ? times_lose[_state - 101] : times_win[_state - 1];
		RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(wait), [this]{ StepForward(); }, false);
	}
}
