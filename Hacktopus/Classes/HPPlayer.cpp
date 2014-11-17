//
//  HPPlayer.cpp
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
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

#include "HPPlayer.h"
#include "HPWorld.h"

namespace HP
{
	RNDefineSingleton(Player)
	
	Player::Player() : _numberOfLifes(4)
	{
		_body = new AnimatableEntity();
		_body->SetDefaultTexture(RN::Texture::WithFile("Textures/hektor2.png"));
		_body->SetPosition(RN::Vector3(0.0f, 300.0f, -4000.0f));
		_body->GetMaterial()->SetLighting(false);
		_body->GetMaterial()->SetAmbientColor(RN::Color::White());
		_body->GetMaterial()->SetDepthWrite(false);
		
		_eyes = new RN::Billboard(RN::Texture::WithFile("Textures/hektor_blinzeln.png"));
		_eyes->SetPosition(RN::Vector3(_body->GetPosition().x, _body->GetPosition().y, -3500.0f));
		_eyes->GetMaterial()->SetLighting(false);
		RN::Timer::ScheduledTimerWithDuration(std::chrono::seconds(3), [&]{
				_eyes->SetPosition(RN::Vector3(_body->GetPosition().x, _body->GetPosition().y, -3500.0f));

			RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(100), [&]{_eyes->SetPosition(RN::Vector3(_body->GetPosition().x, _body->GetPosition().y, -11000.0f));}, false);
				}, true);
		
		_armRight = new AnimatableEntity();
		_armRight->SetDefaultTexture(RN::Texture::WithFile("Textures/tentakelarm+0.png"));
		_armRight->SetPosition(RN::Vector3(500.0f, 290.0f, -3000.0f));
		_armRight->GetMaterial()->SetLighting(false);
		_armRight->GetMaterial()->SetAmbientColor(RN::Color::White());
		_armRight->GetMaterial()->SetDepthWrite(false);
		
		_armLeft = new AnimatableEntity(false);
		_armLeft->SetDefaultTexture(RN::Texture::WithFile("Textures/tentakelarm+0.png"));
		_armLeft->SetPosition(RN::Vector3(-470.0f, 310.0f, -3000.0f));
		_armLeft->GetMaterial()->SetLighting(false);
		_armLeft->GetMaterial()->SetAmbientColor(RN::Color::White());
		_armLeft->GetMaterial()->SetDepthWrite(false);
		_armLeft->GetMaterial()->SetCullMode(RN::Material::CullMode::None);
		
		for(int i = 0; i < 5; i++)
		{
			_lifes[i] = new RN::Billboard(RN::Texture::WithFile("Textures/health_button_green.png"));
			_lifes[i]->GetMaterial()->SetLighting(false);
		}
		
		_lifes[0]->SetPosition(RN::Vector3(-893.0f, 202.0f, -8000.0f));
		_lifes[1]->SetPosition(RN::Vector3(-815.0f, 202.0f, -8000.0f));
		_lifes[2]->SetPosition(RN::Vector3(-737.0f, 202.0f, -8000.0f));
		_lifes[3]->SetPosition(RN::Vector3(-655.0f, 202.0f, -8000.0f));
		_lifes[4]->SetPosition(RN::Vector3(-575.0f, 202.0f, -8000.0f));
	}
	
	Player::~Player()
	{
		
	}
	
	void Player::Reset()
	{
		_numberOfLifes = 4;
		
		for(int i = 0; i < 5; i++)
		{
			_lifes[i]->SetTexture(RN::Texture::WithFile("Textures/health_button_green.png"));
		}
	}
	
	void Player::RemoveLife()
	{
		if(_numberOfLifes < 0)
		{
			World::GetActiveWorld()->Downcast<World>()->PlayOutro(false);
			World::GetActiveWorld()->Downcast<World>()->GameEnded();
			
			return;
		}
		
		_lifes[_numberOfLifes --]->SetTexture(RN::Texture::WithFile("Textures/health_button_red.png"));
	}
	
	void Player::KeepTyping()
	{
		if(!_body->IsPlaying())
			_body->PlayAnimationFile("Animations/hektor_type1.json");
	}
	
	void Player::Attack(Enemy *target)
	{
		if(target->IsKilled())
			return;
		
		RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/test.ogg");
		World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
		
		RN::Vector3 position = target->GetPosition();
		target->kill();
	
		if(position.x > 0)
			_armRight->PlayAnimationFile("Animations/hektor_slap1.json");
		else
			_armLeft->PlayAnimationFile("Animations/hektor_slap1.json");
	}
}