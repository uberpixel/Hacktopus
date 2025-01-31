//
//  HPIntro.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
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

#include "HPIntro.h"
#include "HPWorld.h"

uint32 times[23] = {
	1000, 500, 1500, 2000,
	2000, 1500, 1500,
	2000, 1500, 2500,
	1500, 2000, 1500,
	2500, 2000, 1000,
	2000, 2000, 1500,
	1500, 1500, 2000,
	2000
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
		
		World::GetActiveWorld()->Downcast<World>()->PlayMusic(1);
		
		RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/cough.ogg");
		World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
		
		StepForward();
		Open();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this](RN::Message *message) {
			
			RN::Kernel::GetSharedInstance()->ScheduleFunction([this] {
				
				World::GetActiveWorld()->Downcast<World>()->PlayMusic(0);
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
		stream << "Intro/intro_screens" << index << ".png";
		
		return RN::Texture::WithFile(stream.str());
	}
	
	void Intro::StepForward()
	{
		if(_stopped)
			return;
		
		_state ++;
		
		if(_state >= 23)
		{
			Close();
			_callback();
			
			RN::MessageCenter::GetSharedInstance()->RemoveObserver(this);
			return;
		}
		
		RN::UI::Image *image = new RN::UI::Image(GetIntroImageWithID(_state));
		_imageView->SetImage(image->Autorelease());
		
		RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(times[_state - 1]), [this]{ StepForward(); }, false);
		
		if(_state == 21)
		{
			World::GetActiveWorld()->Downcast<World>()->PlayMusic(0);
			RN::AudioResource *audio = RN::AudioResource::WithFile("Intro/DUNDUNDUUUUUN.ogg");
			World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
		}
	}
}
