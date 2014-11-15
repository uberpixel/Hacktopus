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
#include <sqlite3.h>

namespace HP
{
	class HackingConsole
	{
	public:
		HackingConsole();
		
		void Activate();
		
	private:
		void UpdateLabels();
		void PrintCommand(RN::String *command);
		void PushInput(UniChar character);
		
		std::string RunQuery(const std::string &query) const;
		void PickWord();
		void TakeInput(RN::Event *event);
		void UpdateCharacter();
		
		bool _active;
		bool _wasLastCorrect;
		
		RN::UI::Font *_font;
		RN::UI::Color *_textColor;
		RN::UI::Color *_shadowColor;
		
		RN::String *_text;
		
		RN::RandomNumberGenerator *_generator;
		
		RN::String *_word;
		RN::UniChar _character;
		size_t _index;
		
		sqlite3 *_connection;
		
		RN::UI::Widget *_widget;
		
		RN::UI::Label *_shadow;
		RN::UI::Label *_label;
	};
}

#endif /* defined(__Hacktopus__HPWordProcessor__) */
