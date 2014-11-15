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
		_generator(new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister))
	{
		std::string path = RN::FileManager::GetSharedInstance()->GetFilePathWithName("Words/words.sqlite");
		sqlite3_open(path.c_str(), &_connection);
		
		_font = RN::UI::Font::WithName("Words/Inconsolata-Regular.ttf", 18)->Retain();
		_textColor = RN::UI::Color::WithRGB(0.055, 1, 0)->Retain();
		_shadowColor = RN::UI::Color::WithRGBA(1, 1, 0, 0.2)->Retain();
		_text = new RN::String();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this](RN::Message *message) {
			
			RN::Event *event = message->Downcast<RN::Event>();
			TakeInput(event);
			
		}, this);
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
			
			_widget = new RN::UI::Widget(RN::UI::Widget::Style::Borderless, RN::Rect(374, 121, 348, 201));
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
	
	
	std::string HackingConsole::RunQuery(const std::string &query) const
	{
		sqlite3_stmt *_statement;
		sqlite3_prepare_v2(_connection, query.c_str(), static_cast<int>(query.length()), &_statement, nullptr);
		
		std::string result;
		
		if(sqlite3_step(_statement) == SQLITE_ROW)
		{
			result = std::string(reinterpret_cast<const char *>(sqlite3_column_text(_statement, 0)));
		}
		
		sqlite3_finalize(_statement);
		return result;
	}
	
	void HackingConsole::UpdateCharacter()
	{
		_character = _word->GetCharacterAtIndex(_index);
	}
	
	void HackingConsole::PickWord()
	{
		RN::SafeRelease(_word);
		
		std::stringstream query;
		query << "SELECT lemma FROM words LIMIT 1 OFFSET ";
		query << _generator->GetRandomInt32Range(0, 207235);
		
		std::string result = RunQuery(query.str());
		
		_word  = new RN::String(result.c_str());
		_index = 0;
		
		_wasLastCorrect = true;
		
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
		
		if(character == _character)
		{
			PushInput(character);
			
			_index ++;
			_wasLastCorrect = true;
			
			if(_index == _word->GetLength())
			{
				PickWord();
				return;
			}
			
			UpdateCharacter();
			UpdateLabels();
		}
		else
		{
			_wasLastCorrect = false;
			UpdateCharacter();
		}
	}
}
