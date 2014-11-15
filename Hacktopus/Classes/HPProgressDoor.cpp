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
	
	ProgressDoor::ProgressDoor() : _progress(0.0f), _position(RN::Vector3(-400.0f, 0.0f, -9000.0f))
	{
		SetTexture(RN::Texture::WithFile("Textures/enemy.png"), 1.0f);
		SetPosition(_position);
		GetMaterial()->SetLighting(false);
		
		_offsetFactor = RN::Vector3(GetSize(), 0.0f)/100.0f;
		_offsetFactor.y *= -1.0f;
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
		}
		
		SetPosition(_position+_offsetFactor*_progress);
	}
}