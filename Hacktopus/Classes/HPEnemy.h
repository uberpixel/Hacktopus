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
#include "HPAnimatableEntity.h"

namespace HP
{
	class Enemy : public AnimatableEntity
	{
	public:
		Enemy();
		~Enemy();
		
		void Update(float delta) override;
		void kill();
		
		void GenerateQTE();
		
		bool ConsumeButton(int button);
		bool IsActive() { return _active; }
		
	private:
		static RN::Texture *GetTextureForButton(int button);
		void UpdateQTEItems();
		
		bool _dead;
		bool _active;
		bool _damagedDone;
		
		float _cooldown;
		
		std::vector<int> _qte;
		RN::Array *_qteItems;
		
		RNDeclareMeta(Enemy)
	};
}

#endif /* defined(__Hacktopus__HPEnemy__) */
