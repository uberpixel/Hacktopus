//
//  HPOutro.cpp
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
			World::GetActiveWorld()->Downcast<World>()->PlayMusic(5);
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
