//
//  HPEnemy.h
//  Hacktopus
//
//  Created by Nils Daumann on 14.11.14.
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
		bool IsKilled() { return _killed; }
		
	private:
		static RN::Texture *GetTextureForButton(int button);
		void UpdateQTEItems();
		
		bool _dead;
		bool _active;
		bool _damagedDone;
		bool _killed;
		
		float _cooldown;
		
		std::vector<int> _qte;
		RN::Array *_qteItems;
		RN::Billboard *_shock;
		
		RNDeclareMeta(Enemy)
	};
}

#endif /* defined(__Hacktopus__HPEnemy__) */
