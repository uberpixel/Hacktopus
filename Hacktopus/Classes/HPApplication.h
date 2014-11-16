//
//  HPApplication.h
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __HP_APPLICATION_H__
#define __HP_APPLICATION_H__

#include <Rayne/Rayne.h>
#include "HPWorld.h"
#include "HPIntro.h"

namespace HP
{
	class Application : public RN::Application
	{
	public:
		Application();
		~Application() override;
		
		void RecreateWorld();
		
		void Start() override;
		void WillExit() override;
	};
}

#endif /* __HP_APPLICATION_H__ */
