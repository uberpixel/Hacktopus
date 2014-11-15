//
//  HPAnimatableEntity.h
//  Hacktopus
//
//  Created by Sidney Just on 15/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __Hacktopus__HPAnimatableEntity__
#define __Hacktopus__HPAnimatableEntity__

#include <Rayne/Rayne.h>

namespace HP
{
	class AnimatableEntity : public RN::Billboard
	{
	public:
		AnimatableEntity();
		AnimatableEntity(bool doublesided);
		
		void SetDefaultTexture(RN::Texture *texture);
		
		void Update(float delta) override;
		void PlayAnimation(const std::string &name, float duration = 0.1);
		
	private:
		RN::Texture *AttemptToLoad(const std::string &name, size_t index);
		void UpdateFrame();
		
		RN::Texture *_default;
		
		size_t _index;
		RN::Array *_textures;
		float _duration;
		float _accumulated;
	};
}

#endif /* defined(__Hacktopus__HPAnimatableEntity__) */
