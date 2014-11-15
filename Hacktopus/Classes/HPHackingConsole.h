//
//  HPHackingConsole.h
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __Hacktopus__HPWordProcessor__
#define __Hacktopus__HPWordProcessor__

#include <Rayne/Rayne.h>
#include "HPProgressDoor.h"

namespace HP
{
	class HackingConsole
	{
	public:
		HackingConsole();
		
		void Activate();
		void StartHacking();
		
		void Update(float delta);
		
		bool IsHacking() const { return _hacking; }
		
	private:
		void ParseInput();
		void ShuffleWords();
		
		void UpdateLabels();
		void PrintCommand(RN::String *command);
		void PrintOutput(RN::String *output);
		void PushInput(UniChar character);
		
		void PickWord();
		void TakeInput(RN::Event *event);
		void UpdateCharacter();
		
		bool _active;
		bool _hacking;
		bool _wasLastCorrect;
		
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
