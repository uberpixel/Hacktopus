//
//  HPWorld.h
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
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
		World(bool becomeActiveImmediately = false);
		~World() override;
		
		void LeftFromIntro();
		void PlayOutro(bool win);
		void Reset();
		
		void GameEnded();
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void FinishLoading(RN::Deserializer *deserializer) override;
		
		void PlayMusic(int i);
		
		void Update(float delta) override;
		RN::openal::AudioWorld *GetAudioWorld(){return _audioWorld;}
		void Screenshake(float time = 0.15f, float strength = 0.2f);
		RN::Vector4 GetOrthogonalSize() {return _orthogonalSize;}
		bool IsInIntro(){return _inIntro;}
		
	private:
		bool _ended;
		bool _inIntro;
		bool _activeImmediately;
		RN::Array *_enemies;
		
		RN::Camera *_camera;
		RN::Vector4 _orthogonalSize;
		HackingConsole *_console;
		RN::RandomNumberGenerator *_rng;
		float _time;
		float _shakeTime;
		float _shakeStrength;
		RN::openal::AudioWorld *_audioWorld;
		RN::openal::AudioSource *_musicSource;
		RN::GamepadDevice *_gamepad;
		
		uint32_t _pressed;
	};
}

#endif /* __HP_WORLD_H__ */
