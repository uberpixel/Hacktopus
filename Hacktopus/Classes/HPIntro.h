//
//  HPIntro.h
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __Hacktopus__HPIntro__
#define __Hacktopus__HPIntro__

#include <Rayne/Rayne.h>

namespace HP
{
	class Intro : public RN::UI::Widget
	{
	public:
		Intro();
		~Intro();

		void Play(RN::Function &&f);
		
	private:
		void StepForward();
		int _state;
		RN::Function _callback;
		RN::UI::ImageView *_imageView;
	};
}

#endif /* defined(__Hacktopus__HPIntro__) */
