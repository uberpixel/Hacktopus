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
		_hacking(false),
		_input(nullptr),
		_generator(new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister)),
		_wordlistIndex(0),
		_cooldown(0.0f)
	{
		_wordlist = RN::JSONSerialization::JSONObjectFromData<RN::Array>(RN::Data::WithContentsOfFile("Words/words.json"));
		_wordlist->Retain();
		
		_font = RN::UI::Font::WithName("Words/Inconsolata-Regular.ttf", 18)->Retain();
		_textColor = RN::UI::Color::WithRGB(0.055, 1, 0)->Retain();
		_shadowColor = RN::UI::Color::WithRGB(0.055*0.8f, 1*0.8f, 0.2f)->Retain();
		_text = new RN::String();
		
		ShuffleWords();
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputEventMessage, [this](RN::Message *message) {
			
			RN::Event *event = message->Downcast<RN::Event>();
			TakeInput(event);
			
		}, this);
	}
	
	void HackingConsole::Update(float delta)
	{
		_cooldown -= delta;
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
			
			RN::Vector2 resolution = RN::Window::GetSharedInstance()->GetSize();
			RN::Rect widgetRect(333, 110, 750, 300);
			widgetRect.x /= 960.0f;
			widgetRect.y /= 600.0f;
			widgetRect.x -= 0.5f;
			widgetRect.y -= 0.5f;
			widgetRect.x *= resolution.x;
			widgetRect.y *= resolution.y;
			widgetRect.x += resolution.x/2.0f;
			widgetRect.y += resolution.y/2.0f;
			widgetRect.width *= resolution.x/960.0f;
			widgetRect.height *= resolution.y/600.0f;
			
			if(RN::Math::Compare(resolution.y, 360))
			{
				widgetRect.x -= 50.0f;
				widgetRect.y -= 100.0f;
				widgetRect.height += 100.0f;
			}
			_widget = new RN::UI::Widget(RN::UI::Widget::Style::Borderless, widgetRect);
			_widget->GetContentView()->AddSubview(_shadow);
			_widget->GetContentView()->AddSubview(_label);
			_widget->GetContentView()->SetBackgroundColor(RN::UI::Color::ClearColor());
			_widget->Open();
		
			PrintCommand(RNCSTR("Ultracorp 4000 Mainframe - Welcome"));
			PrintCommand(RNCSTR("Select: [n]ew game, [h]elp, [e]xit"));
		}
		
		_active  = true;
		_hacking = false;
		
		if(!_input)
			_input = new RN::String();
	}
	
	void HackingConsole::StartHacking()
	{
		_hacking = true;
		
		PrintCommand(RNCSTR("cd /sbin/iggj7/"));
		PrintCommand(RNCSTR("./masterhack"));
		
		PickWord();
	}
	
	void HackingConsole::ParseInput()
	{
		PrintOutput(_input);
		
		if(_input->IsEqual(RNCSTR("n")))
		{
			StartHacking();
		}
		else if(_input->IsEqual(RNCSTR("h")) || _input->IsEqual(RNCSTR("help")))
		{
			PrintOutput(RNCSTR("Hack the mainframe by typing the"));
			PrintOutput(RNCSTR("hacker commands. Slap the pesky"));
			PrintCommand(RNCSTR("scientists before they get to you"));
		}
		else if(_input->IsEqual(RNCSTR("e")))
		{
			RN::Kernel::GetSharedInstance()->Exit();
		}
		else if(_input->IsEqual(RNCSTR("ls")))
		{
			PrintOutput(RNCSTR("."));
			PrintOutput(RNCSTR(".."));
			PrintCommand(RNCSTR("masterhack"));
		}
		else if(_input->GetRangeOfString(RNCSTR("cat")).origin == 0)
		{
			PrintCommand(RNCSTR("meow meow meow"));
		}
		else
		{
			PrintCommand(RNSTR("Unrecognized command '%s'", _input->GetUTF8String()));
		}
		
		RN::SafeRelease(_input);
		_input = new RN::String();
		
		UpdateLabels();
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
		
		RN::String *final = _text->Copy();
		if(!_hacking && _input)
			final->Append(_input);
		
		_shadow->SetText(shadow);
		_label->SetText(final->Autorelease());
	}
	
	void HackingConsole::PrintOutput(RN::String *output)
	{
		_text->Append(output);
		_text->Append("\n");
		
		UpdateLabels();
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
		
		if(_wordlistIndex == 0)
			ShuffleWords();
		
		UpdateCharacter();
	}
	
	void HackingConsole::TakeInput(RN::Event *event)
	{
		if(!_active)
			return;
		
		if(event->GetType() != RN::Event::Type::KeyDown)
			return;
		
		RN::UniChar character = event->GetCharacter();
		
		if(!_hacking)
		{
			if(RN::CodePoint(character).IsNewline())
			{
				ParseInput();
				return;
			}
			
			if(character == 63)
			{
				if(_input->GetLength() > 0)
				{
					_input->ReplaceCharacters(RNCSTR(""), RN::Range(_input->GetLength() - 1, 1));
					UpdateLabels();
				}
				
				return;
			}
			
			_input->Append(RNSTR("%c", character));
			UpdateLabels();
			return;
		}
		
		
		// Whitespace is optional
		if(RN::CodePoint(_character).IsWhitespace() && !RN::CodePoint(character).IsWhitespace())
		{
			if(RN::CodePoint(_word->GetCharacterAtIndex(_index + 1)).GetLowerCase() == RN::CodePoint(character).GetLowerCase())
			{
				_index ++;
				_character = _word->GetCharacterAtIndex(_index);
				
				PushInput(RN::CodePoint::ConvertCharacter(' '));
			}
		}
		
		// Enter at the beginning is optional too
		if(_index == 0 && RN::CodePoint(character).IsNewline())
			return;
		
		if(RN::CodePoint(character).GetLowerCase() == RN::CodePoint(_character).GetLowerCase())
		{
			PushInput(_character);
			
			_index ++;
			_wasLastCorrect = true;
			
			if(_index == _word->GetLength())
			{
				ProgressDoor::GetSharedInstance()->Progress(10.0f);
				PickWord();
				
				PrintCommand(RNCSTR(""));
				return;
			}
			
			UpdateCharacter();
			UpdateLabels();
		}
		else
		{
			if(_cooldown <= 0.0f)
				ProgressDoor::GetSharedInstance()->Progress(-5.0f);
			
			_wasLastCorrect = false;
			_cooldown = 1.25f;
			
			UpdateCharacter();
		}
	}
}
