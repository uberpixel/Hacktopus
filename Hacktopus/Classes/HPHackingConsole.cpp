//
//  HackingConsole.cpp
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPHackingConsole.h"

namespace HP
{
	HackingConsole::HackingConsole() :
		_active(false),
		_word(nullptr),
		_widget(nullptr),
		_generator(new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister)),
		_wordlistIndex(0)
	{
		_wordlist = RN::JSONSerialization::JSONObjectFromData<RN::Array>(RN::Data::WithContentsOfFile("Words/words.json"));
		_wordlist->Retain();
		
		_font = RN::UI::Font::WithName("Words/Inconsolata-Regular.ttf", 18)->Retain();
		_textColor = RN::UI::Color::WithRGB(0.055, 1, 0)->Retain();
		_shadowColor = RN::UI::Color::WithRGBA(0.047, 0.588, 0, 0.2)->Retain();
		_text = new RN::String();
		
		ShuffleWords();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this](RN::Message *message) {
			
			RN::Event *event = message->Downcast<RN::Event>();
			TakeInput(event);
			
		}, this);
	}
	
	void HackingConsole::ShuffleWords()
	{
		// Brain dead Fisher-Yates shuffle... From memory
		
		RN::Random::MersenneTwister random;
		
		int32 count = static_cast<int32>(_wordlist->GetCount());
		for(int32 i = count - 1; i >= 0; i --)
		{
			size_t index = random.GetRandomInt32Range(0, i + 1);
			
			RN::Object *a = _wordlist->GetObjectAtIndex(index)->Retain();
			RN::Object *b = _wordlist->GetObjectAtIndex(i)->Retain();
			
			_wordlist->ReplaceObjectAtIndex(index, b);
			_wordlist->ReplaceObjectAtIndex(i, a);
			
			a->Release();
			b->Release();
		}
	}
	
	void HackingConsole::Activate()
	{
		if(_active)
			return;
		
		if(!_widget)
		{
			_label = new RN::UI::Label();
			_label->SetFrame(RN::Rect(0, 0, 348, 201).Inset(5, 5));
			_label->SetFont(_font);
			_label->SetTextColor(_textColor);
			_label->SetText(_text);
			_label->SetNumberOfLines(0);
			_label->SetLineBreak(RN::UI::LineBreakMode::TruncateHead);
			
			_shadow = new RN::UI::Label();
			_shadow->SetFrame(RN::Rect(0, 0, 348, 201).Inset(5, 5));
			_shadow->SetFont(_font);
			_shadow->SetTextColor(_shadowColor);
			_shadow->SetText(_text);
			_shadow->SetNumberOfLines(0);
			_shadow->SetLineBreak(RN::UI::LineBreakMode::TruncateHead);
			
			_widget = new RN::UI::Widget(RN::UI::Widget::Style::Borderless, RN::Rect(374, 100, 348, 201));
			_widget->GetContentView()->AddSubview(_shadow);
			_widget->GetContentView()->AddSubview(_label);
			_widget->GetContentView()->SetBackgroundColor(RN::UI::Color::Black());
			_widget->Open();
		
			PrintCommand(RNCSTR("Ultracorp 4000 Mainframe - Welcome"));
			PrintCommand(RNCSTR("cd /sbin/iggj7/"));
			PrintCommand(RNCSTR("./masterhack"));
		}
		
		_active = true;
		PickWord();
	}
	
	
	void HackingConsole::UpdateLabels()
	{
		RN::Array *lines = _text->GetComponentsSeparatedByString(RNCSTR("\n"));
		if(lines->GetCount() >= 10)
		{
			RN::SafeRelease(_text);
			_text = new RN::String();
			
			lines->Enumerate<RN::String>([&](RN::String *line, size_t index, bool &stop) {
			
				if(index == 0)
					return;
				
				if(index > 1)
					_text->Append("\n");
				
				_text->Append(line);
				
			});
		}
		
		RN::String *shadow = _text->Copy()->Autorelease();
		
		if(_word)
		{
			RN::String *left = _word->GetSubstring(RN::Range(_index, _word->GetLength() - _index));
			shadow->Append(left);
		}
		
		_shadow->SetText(shadow);
		_label->SetText(_text);
	}
	
	void HackingConsole::PrintCommand(RN::String *command)
	{
		_text->Append(command);
		_text->Append("\n> ");
		
		UpdateLabels();
	}
	
	void HackingConsole::PushInput(UniChar character)
	{
		_text->Append(RN::String::WithFormat("%c", static_cast<char>(character)));
		
		UpdateLabels();
	}
	
	void HackingConsole::UpdateCharacter()
	{
		_character = _word->GetCharacterAtIndex(_index);
	}
	
	void HackingConsole::PickWord()
	{
		_word = _wordlist->GetObjectAtIndex<RN::String>(_wordlistIndex);
		_index = 0;
		_wasLastCorrect = true;
		
		_wordlistIndex = (_wordlistIndex + 1) % _wordlist->GetCount();
		
		UpdateCharacter();
		PrintCommand(RNCSTR(""));
	}
	
	void HackingConsole::TakeInput(RN::Event *event)
	{
		if(!_active)
			return;
		
		if(event->GetType() != RN::Event::Type::KeyDown)
			return;
		
		RN::UniChar character = event->GetCharacter();
		
		if(RN::CodePoint(character).GetLowerCase() == RN::CodePoint(_character).GetLowerCase())
		{
			PushInput(_character);
			
			_index ++;
			_wasLastCorrect = true;
			
			if(_index == _word->GetLength())
			{
				ProgressDoor::GetSharedInstance()->Progress(10.0f);
				PickWord();
				return;
			}
			
			UpdateCharacter();
			UpdateLabels();
		}
		else
		{
			ProgressDoor::GetSharedInstance()->Progress(-10.0f);
			_wasLastCorrect = false;
			UpdateCharacter();
		}
	}
}
