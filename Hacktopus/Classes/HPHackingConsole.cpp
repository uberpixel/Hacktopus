//
//  HackingConsole.cpp
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPHackingConsole.h"
#include "HPWorld.h"

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
	
	HackingConsole::~HackingConsole()
	{
		_wordlist->Release();
		_generator->Release();
		
		RN::SafeRelease(_widget);
		RN::SafeRelease(_font);
		RN::SafeRelease(_textColor);
		RN::SafeRelease(_shadowColor);
		RN::SafeRelease(_text);
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
		for(int32 i = count - 1; i > 0; i --)
		{
			size_t index = random.GetRandomInt32Range(0, i);
			
			RN::Object *a = _wordlist->GetObjectAtIndex(index)->Retain();
			RN::Object *b = _wordlist->GetObjectAtIndex(i)->Retain();
			
			_wordlist->ReplaceObjectAtIndex(index, b);
			_wordlist->ReplaceObjectAtIndex(i, a);
		}
	}
	
	void HackingConsole::Deactivate()
	{
		if(!_active)
			return;
		
		_widget->Close();
		_widget->Release();
		_widget = nullptr;
		
		RN::SafeRelease(_text);
		
		_text = new RN::String();
		_active = false;
		
		_canSpawn = false;
		
		_wordlist->Release();
		_wordlist = RN::JSONSerialization::JSONObjectFromData<RN::Array>(RN::Data::WithContentsOfFile("Words/words.json"));
		_wordlist->Retain();
		
		_wasLastCorrect = true;
		_wordlistIndex = 0;
		_index = 0;
		
		RN::SafeRelease(_input);
		_input = new RN::String();
		
		RN::SafeRelease(_word);
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
			
			_widget = new RN::UI::Widget(RN::UI::Widget::Style::Borderless, widgetRect);
			_widget->GetContentView()->AddSubview(_shadow->Autorelease());
			_widget->GetContentView()->AddSubview(_label->Autorelease());
			_widget->GetContentView()->SetBackgroundColor(RN::UI::Color::ClearColor());
			_widget->Open();
		
			PrintCommand(RNCSTR("Ultracorp 4000 Mainframe - Welcome"));
			PrintCommand(RNCSTR("Select: [n]ew game, [h]elp, [e]xit"));
			PrintCommand(RNCSTR("Confirm with [enter]"));
			
			ShuffleWords();
		}
		
		_active  = true;
		_hacking = false;
		
		if(!_input)
			_input = new RN::String();
	}
	
	void HackingConsole::StartHacking()
	{
		_hacking = true;
		_canSpawn = false;
		
		World::GetActiveWorld()->Downcast<World>()->PlayMusic(0);
		
		RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/computer_start.ogg");
		World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
		
		//PrintCommand(RNCSTR("cd /sbin/iggj7/"));
		//PrintCommand(RNCSTR("./masterhack"));
		
		PrintCommand(RNCSTR("Type what you see on the screen!"));
		
		_wordlist->InsertObjectAtIndex(RNCSTR("type this"), 0);
		_wordlist->InsertObjectAtIndex(RNCSTR("hatch unlocked and opened"), 1);
		_wordlist->InsertObjectAtIndex(RNCSTR("click on the angry scientists"), 2);
		
		PickWord();
	}
	
	void HackingConsole::ParseInput()
	{
		PrintOutput(_input);
		
		if(_input->IsEqual(RNCSTR("n")) || _input->IsEqual(RNCSTR("new game")) || _input->IsEqual(RNCSTR("./masterhack")))
		{
			StartHacking();
		}
		else if(_input->IsEqual(RNCSTR("h")) || _input->IsEqual(RNCSTR("help")))
		{
			PrintOutput(RNCSTR("Hack the mainframe by typing the"));
			PrintOutput(RNCSTR("hacker commands. Slap the pesky"));
			PrintOutput(RNCSTR("scientists before they get to you."));
			PrintOutput(RNCSTR("Case doesn't matter."));
			PrintCommand(RNCSTR("Are you a hacker? Try ls!"));
		}
		else if(_input->IsEqual(RNCSTR("e")))
		{
			RN::Kernel::GetSharedInstance()->Exit();
		}
		else if(_input->IsEqual(RNCSTR("ls")))
		{
			PrintOutput(RNCSTR("damage"));
			PrintOutput(RNCSTR("dundun"));
			PrintOutput(RNCSTR("lose"));
			PrintOutput(RNCSTR("masterhack"));
			PrintCommand(RNCSTR("win"));
		}
		else if(_input->GetRangeOfString(RNCSTR("cat")).origin == 0)
		{
			PrintCommand(RNCSTR("meow meow meow"));
		}
		else if(_input->IsEqual(RNCSTR("./win")))
		{
			World::GetActiveWorld()->Downcast<World>()->PlayOutro(true);
		}
		else if(_input->IsEqual(RNCSTR("./lose")))
		{
			World::GetActiveWorld()->Downcast<World>()->PlayOutro(false);
		}
		else if(_input->IsEqual(RNCSTR("./damage")))
		{
			Player::GetSharedInstance()->RemoveLife();
			PrintCommand(RNCSTR("Ouch"));
		}
		else if(_input->IsEqual(RNCSTR("./dundun")))
		{
			RN::AudioResource *audio = RN::AudioResource::WithFile("Intro/DUNDUNDUUUUUN.ogg");
			World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
			PrintCommand(RNCSTR(""));
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
		_text->Append("\n ");
		
		UpdateLabels();
	}
	
	void HackingConsole::PrintCommand(RN::String *command)
	{
		_text->Append(command);
		_text->Append("\n> ");
		
		UpdateLabels();
	}
	
	void HackingConsole::PushInput(RN::UniChar character)
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
		
		if(!_canSpawn && _wordlistIndex > 2)
		{
			_canSpawn = true;
			World::GetActiveWorld()->Downcast<World>()->PlayMusic(2);
		}
		
		if(_wordlistIndex == 0)
		{
			for(int i = 0; i < 3; i ++)
				_wordlist->RemoveObjectAtIndex(0);
			
			ShuffleWords();
		}
		
		UpdateCharacter();
	}
	
	void HackingConsole::TakeInput(RN::Event *event)
	{
		if(World::GetActiveWorld()->Downcast<World>()->IsInIntro())
		{
			_hacking = false;
			return;
		}
		
		if(!_active)
			return;
		
		if(event->GetType() != RN::Event::Type::KeyDown)
			return;
		
		Player::GetSharedInstance()->KeepTyping();
		
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
				RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/console_start.ogg");
				World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
				
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
			if(!_canSpawn)
				return;
			
			if(_cooldown <= 0.0f)
			{
				ProgressDoor::GetSharedInstance()->Progress(-5.0f);
				
				RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/console_error1.ogg");
				World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
			}
			
			_wasLastCorrect = false;
			_cooldown = 1.25f;
			
			UpdateCharacter();
		}
	}
}
