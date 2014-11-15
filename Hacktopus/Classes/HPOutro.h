//
//  HPOutro.h
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __Hacktopus__HPOutro__
#define __Hacktopus__HPOutro__

#include <Rayne/Rayne.h>

namespace HP
{
	class Outro : public RN::UI::Widget
	{
	public:
		Outro();
		~Outro();

		void Play(RN::Function &&f, bool win);
		
	private:
		RN::Texture *GetOutroImageWithID(size_t index);
		void StepForward();
		
		int _state;
		RN::Function _callback;
		RN::UI::ImageView *_imageView;
	};
}

#endif /* defined(__Hacktopus__HPOutro__) */
