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
		_textures(nullptr),
		_default(nullptr),
		_duration(0),
		_accumulated(0),
		_index(0)
	{}
	
	AnimatableEntity::AnimatableEntity(bool doublesided) :
		RN::Billboard(doublesided),
		_textures(nullptr),
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
		
		RN::SafeRelease(_textures);
		
		size_t i = 0;
		while(1)
		{
			RN::Texture *texture = AttemptToLoad(name, i ++);
			if(!texture)
				break;
			
			if(!_textures)
				_textures = new RN::Array();
			
			_textures->AddObject(texture);
		}
		
		UpdateFrame();
	}
	
	void AnimatableEntity::UpdateFrame()
	{
		if(!_textures)
		{
			SetTexture(_default);
			return;
		}
		
		RN::Texture *texture = _textures->GetObjectAtIndex<RN::Texture>(_index);
		SetTexture(texture);
	}
	
	void AnimatableEntity::Update(float delta)
	{
		RN::Billboard::Update(delta);
		
		if(!_textures)
			return;
		
		_accumulated += delta;
		if(_accumulated >= _duration)
		{
			_accumulated -= _duration;
			_index = ((_index + 1) % _textures->GetCount());
			
			if(_index == 0)
				RN::SafeRelease(_textures);
			
			UpdateFrame();
		}
	}
}
