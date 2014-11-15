//
//  HPPlayer.h
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __Hacktopus__HPPlayer__
#define __Hacktopus__HPPlayer__

#include <Rayne/Rayne.h>

namespace HP
{
	class Player : public RN::ISingleton<Player>
	{
	public:
		Player();
		~Player();
		
		void Attack(RN::SceneNode *target);
		
	private:
		RN::Billboard *_body;
		RN::Billboard *_armRight;
		RN::Billboard *_armLeft;
		
		RNDeclareSingleton(Player)
	};
	
	
}

#endif /* defined(__Hacktopus__HPPlayer__) */
