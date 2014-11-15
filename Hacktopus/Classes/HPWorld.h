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

namespace HP
{
	class World : public RN::World
	{
	public:
		World();
		~World() override;
		
		void LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer) override;
		void Update(float delta) override;
		void FinishLoading(RN::Deserializer *deserializer) override;
		
		
	private:
		RN::Camera *_camera;
		HackingConsole *_console;
		RN::RandomNumberGenerator *_rng;
		float _time;
	};
}

#endif /* __HP_WORLD_H__ */
