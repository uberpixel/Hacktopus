//
//  HPEnemy.h
//  Hacktopus
//
//  Created by Nils Daumann on 14.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __Hacktopus__HPEnemy__
#define __Hacktopus__HPEnemy__

#include <Rayne/Rayne.h>

namespace HP
{
	class Enemy : public RN::Billboard
	{
	public:
		Enemy();
		~Enemy();
		
		void Update(float delta) override;
		void kill();
		
	private:
		bool _dead;
		
		RNDeclareMeta(Enemy)
	};
}

#endif /* defined(__Hacktopus__HPEnemy__) */
