//
//  HPHackingConsole.h
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

#ifndef __Hacktopus__HPWordProcessor__
#define __Hacktopus__HPWordProcessor__

#include <Rayne/Rayne.h>
#include "HPProgressDoor.h"
#include "HPPlayer.h"

namespace HP
{
	class HackingConsole
	{
	public:
		HackingConsole();
		~HackingConsole();
		
		void Activate();
		void Deactivate();
		
		void StartHacking();
		
		void Update(float delta);
		
		bool IsHacking() const { return _hacking; }
		bool CanSpawn() const { return _canSpawn; }
		
	private:
		void ParseInput();
		void ShuffleWords();
		
		void UpdateLabels();
		void PrintCommand(RN::String *command);
		void PrintOutput(RN::String *output);
		void PushInput(RN::UniChar character);
		
		void PickWord();
		void TakeInput(RN::Event *event);
		void UpdateCharacter();
		
		bool _active;
		bool _hacking;
		bool _wasLastCorrect;
		bool _canSpawn;
		
		float _cooldown;
		
		RN::UI::Font *_font;
		RN::UI::Color *_textColor;
		RN::UI::Color *_shadowColor;
		
		RN::String *_text;
		RN::String *_input;
		
		RN::RandomNumberGenerator *_generator;
		
		RN::String *_word;
		RN::UniChar _character;
		size_t _index;
		
		RN::Array *_wordlist;
		size_t _wordlistIndex;
		
		RN::UI::Widget *_widget;
		
		RN::UI::Label *_shadow;
		RN::UI::Label *_label;
	};
}

#endif /* defined(__Hacktopus__HPWordProcessor__) */
