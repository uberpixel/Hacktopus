//
//  HPPlayer.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPPlayer.h"

namespace HP
{
	RNDefineSingleton(Player)
	
	Player::Player()
	{
		
	}
	
	Player::~Player()
	{
		
	}
	
	void Player::Attack(RN::SceneNode *target)
	{
		target->RemoveFromWorld();
	}
}