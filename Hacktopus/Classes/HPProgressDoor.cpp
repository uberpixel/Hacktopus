//
//  HPProgressDoor.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPProgressDoor.h"

namespace HP
{
	RNDefineSingleton(ProgressDoor)
	
	ProgressDoor::ProgressDoor() : _progress(0.0f)
	{
		SetTexture(RN::Texture::WithFile("Textures/enemy.png"), 1.0f);
		SetPosition(RN::Vector3(-300.0f, 0.0f, -9000.0f));
		GetMaterial()->SetLighting(false);
	}
	
	ProgressDoor::~ProgressDoor()
	{
		
	}
	
	void ProgressDoor::Progress(float progress)
	{
		_progress += progress;
		if(_progress < 0.0f)
		{
			_progress = 0.0f;
			progress = 0.0f;
		}
		
		Translate(RN::Vector3(progress, -progress, 0.0f));
	}
}