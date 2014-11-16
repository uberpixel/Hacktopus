//
//  HPProgressDoor.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPProgressDoor.h"
#include "HPWorld.h"

namespace HP
{
	RNDefineSingleton(ProgressDoor)
	
	ProgressDoor::ProgressDoor() : _progress(0.0f), _position(RN::Vector3(-745.0f, -26.0f, -12000.0f)), _isOpened(false)
	{
		_topDoor = new RN::Billboard();
		_topDoor->SetTexture(RN::Texture::WithFile("Textures/door2_400x360.png"), 1.0f);
		_topDoor->SetPosition(_position);
		_topDoor->GetMaterial()->SetLighting(false);
		_bottomDoor = new RN::Billboard();
		_bottomDoor->SetTexture(RN::Texture::WithFile("Textures/door1_400x360.png"), 1.0f);
		_bottomDoor->SetPosition(_position);
		_bottomDoor->GetMaterial()->SetLighting(false);
		
		_offsetFactor = RN::Vector3(_topDoor->GetSize(), 0.0f)/200.0f;
		_offsetFactor.y *= -1.0f;
	}
	
	ProgressDoor::~ProgressDoor()
	{
		
	}
	
	void ProgressDoor::Reset()
	{
		_isOpened = false;
		Progress(-_progress);
	}
	
	void ProgressDoor::Progress(float progress)
	{
		if(_isOpened)
			return;
		
		_progress += progress;
		_progress = std::max(_progress, 0.0f);
		
		if(_progress >= 100.0f)
		{
			_isOpened = true;
			
			World::GetActiveWorld()->Downcast<World>()->GameEnded();
			
			RN::Billboard *medizin = new RN::Billboard();
			medizin->SetTexture(RN::Texture::WithFile("Textures/medizin.png"));
			medizin->GetMaterial()->SetLighting(false);
			medizin->SetPosition(RN::Vector3(-738.0f, -200.0f, -14000.0f));
			medizin->SetAction([](RN::SceneNode *medizin, float delta){
				if(medizin->GetPosition().y < -26.0f)
					medizin->Translate(RN::Vector3(0.0f, 200.0f*delta, 0.0f));
			});
			
			RN::Timer::ScheduledTimerWithDuration(std::chrono::seconds(4), []{World::GetActiveWorld()->Downcast<World>()->PlayOutro(true);}, false);
		}
		
		RN::Vector3 topPosition = _position+_offsetFactor*_progress;
		topPosition.x = roundf(topPosition.x);
		topPosition.y = roundf(topPosition.y);
		topPosition.z = roundf(topPosition.z);
		
		RN::Vector3 bottomPosition = _position-_offsetFactor*_progress;
		bottomPosition.x = roundf(bottomPosition.x);
		bottomPosition.y = roundf(bottomPosition.y);
		bottomPosition.z = roundf(bottomPosition.z);
		
		_topDoor->SetPosition(topPosition);
		_bottomDoor->SetPosition(bottomPosition);
	}
}