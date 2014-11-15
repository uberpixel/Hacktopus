//
//  HPWorld.h
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __HP_WORLD_H__
#define __HP_WORLD_H__

#include <Rayne/Rayne.h>
#include "HPHackingConsole.h"
#include "HPEnemy.h"
#include "HPPlayer.h"
#include "HPProgressDoor.h"
#include "HPOutro.h"
#include <RALAudioWorld.h>

namespace HP
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void LeftFromIntro();
		void PlayOutro(bool win);
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void Update(float delta) override;
		RN::openal::AudioWorld *GetAudioWorld(){return _audioWorld;}
		void Screenshake(float time = 0.15f, float strength = 0.2f);
		RN::Vector4 GetOrthogonalSize() {return _orthogonalSize;}
		bool IsInIntro(){return _inIntro;}
		
	private:
		bool _inIntro;
		RN::Array *_enemies;
		
		RN::Camera *_camera;
		RN::Vector4 _orthogonalSize;
		HackingConsole *_console;
		RN::RandomNumberGenerator *_rng;
		float _time;
		float _shakeTime;
		float _shakeStrength;
		RN::openal::AudioWorld *_audioWorld;
		RN::GamepadDevice *_gamepad;
		
		uint32_t _pressed;
	};
}

#endif /* __HP_WORLD_H__ */
