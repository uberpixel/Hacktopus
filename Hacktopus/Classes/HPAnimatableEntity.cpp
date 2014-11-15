//
//  HPAnimatableEntity.cpp
//  Hacktopus
//
//  Created by Sidney Just on 15/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPAnimatableEntity.h"

namespace HP
{
	AnimatableEntity::AnimatableEntity() :
		_frames(nullptr),
		_default(nullptr),
		_duration(0),
		_accumulated(0),
		_index(0)
	{}
	
	AnimatableEntity::AnimatableEntity(bool doublesided) :
		RN::Billboard(doublesided),
		_frames(nullptr),
		_default(nullptr),
		_duration(0),
		_accumulated(0),
		_index(0)
	{}
	
	void AnimatableEntity::SetDefaultTexture(RN::Texture *texture)
	{
		RN::SafeRelease(_default);
		_default = texture->Retain();
		
		UpdateFrame();
	}
	
	RN::Texture *AnimatableEntity::AttemptToLoad(const std::string &name, size_t index)
	{
		try
		{
			std::stringstream complete;
			complete << name << "+" << index << ".png";
			
			RN::Texture *texture = RN::Texture::WithFile(complete.str());
			return texture;
		}
		catch(RN::Exception e)
		{}
		
		return nullptr;
	}
	
	void AnimatableEntity::PlayAnimation(const std::string &name, float duration)
	{
		_duration = duration;
		_accumulated = 0;
		_index = 0;
		
		RN::SafeRelease(_frames);
		
		size_t i = 0;
		while(1)
		{
			RN::Texture *texture = AttemptToLoad(name, i ++);
			if(!texture)
				break;
			
			if(!_frames)
				_frames = new RN::Array();
			
			RN::Dictionary *processed = new RN::Dictionary();
			processed->SetObjectForKey(texture, RNCSTR("texture"));
			processed->SetObjectForKey(RN::Number::WithDouble(duration), RNCSTR("duration"));
			
			_frames->AddObject(processed->Autorelease());
		}
		
		UpdateFrame();
	}
	
	void AnimatableEntity::PlayAnimationFile(const std::string &name)
	{
		_accumulated = 0;
		_index = 0;
		
		RN::SafeRelease(_frames);
		
		_frames = RN::JSONSerialization::JSONObjectFromData<RN::Array>(RN::Data::WithContentsOfFile(name))->Retain();
		_frames->Enumerate<RN::Dictionary>([&](RN::Dictionary *dictionary, size_t index, bool &stop) {
			
			RN::String *file = dictionary->GetObjectForKey<RN::String>(RNCSTR("file"));
			RN::Texture *texture = RN::Texture::WithFile(file->GetUTF8String());
			
			dictionary->SetObjectForKey(texture, RNCSTR("texture"));
			
		});
		
		UpdateFrame();
	}
	
	
	void AnimatableEntity::UpdateFrame()
	{
		if(!_frames)
		{
			SetTexture(_default);
			return;
		}
		
		RN::Dictionary *frame = _frames->GetObjectAtIndex<RN::Dictionary>(_index);
		
		RN::Texture *texture = frame->GetObjectForKey<RN::Texture>(RNCSTR("texture"));
		RN::Number *duration = frame->GetObjectForKey<RN::Number>(RNCSTR("duration"));
		
		SetTexture(texture);
		_duration = duration->GetDoubleValue();
	}
	
	void AnimatableEntity::Update(float delta)
	{
		RN::Billboard::Update(delta);
		
		if(!_frames)
			return;
		
		_accumulated += delta;
		if(_accumulated >= _duration)
		{
			_accumulated -= _duration;
			_index = ((_index + 1) % _frames->GetCount());
			
			if(_index == 0)
				RN::SafeRelease(_frames);
			
			UpdateFrame();
		}
	}
}
