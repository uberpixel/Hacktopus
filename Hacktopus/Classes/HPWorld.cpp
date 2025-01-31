//
//  HPWorld.cpp
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

#include "HPWorld.h"
#include "HPApplication.h"

namespace HP
{
	World::World(bool becomeActive) :
		RN::World("GenericSceneManager"),
		_gamepad(nullptr),
		_enemies(new RN::Array()),
		_ended(false),
		_console(nullptr),
		_audioWorld(nullptr),
		_activeImmediately(becomeActive),
		_time(0),
		_pressed(0),
		_shakeStrength(0),
		_shakeTime(0),
		_musicSource(nullptr)
	{
		_rng = new RN::RandomNumberGenerator(RN::RandomNumberGenerator::Type::MersenneTwister);
		_inIntro = true;
		
		_audioWorld = new RN::openal::AudioWorld();
		AddAttachment(_audioWorld);
		
		RN::MessageCenter::GetSharedInstance()->AddObserver(kRNInputInputDeviceRegistered, [&](RN::Message *message) {
			
			if(_gamepad)
				return;
			
			_gamepad = message->GetObject()->Downcast<RN::GamepadDevice>();
			_gamepad->Retain();
			_gamepad->Activate();
			
		}, this);
		
		RN::Array *devices = RN::Input::GetSharedInstance()->GetInputDevicesMatching(RN::InputDevice::Category::Gamepad);
		if(devices->GetCount() > 0)
		{
			_gamepad = devices->GetObjectAtIndex<RN::GamepadDevice>(0);
			_gamepad->Retain();
			_gamepad->Activate();
		}
	}
	
	World::~World()
	{
		if(_audioWorld)
			_audioWorld->Autorelease();
		
		if(_gamepad)
		{
			_gamepad->Deactivate();
			_gamepad->Release();
		}
		
		_rng->Release();
		_enemies->Release();
		
		delete _console;
	}
	
	void World::Reset()
	{
		Player::GetSharedInstance()->Reset();
		ProgressDoor::GetSharedInstance()->Reset();
		
		_console->Deactivate();
		_console->Activate();
		
		_inIntro = false;
		_ended = false;
		
		_enemies->Enumerate<RN::SceneNode>([](RN::SceneNode *node, size_t index, bool &stop) {
			node->RemoveFromWorld();
		});
		_enemies->RemoveAllObjects();
	}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		RN::openal::AudioListener *audioListener = new RN::openal::AudioListener();
		
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, (RN::Camera::Flags::Defaults | RN::Camera::Flags::Orthogonal) & ~RN::Camera::Flags::UseFog);
		_camera->AddAttachment(audioListener);
	}
	
	void World::FinishLoading(RN::Deserializer *deserializer)
	{
		if(_activeImmediately)
			LeftFromIntro();
	}
	
	void World::PlayMusic(int i)
	{
		if(i == 0 && _musicSource)
		{
			_musicSource->SetSelfdestruct(true);
			_musicSource->Stop();
			_musicSource = nullptr;
		}
		if(i == 1)
		{
			RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/music_intro_141019_HossaHossa.ogg");
			_musicSource = World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
			_musicSource->SetRepeat(true);
		}
		if(i == 2)
		{
			RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/music_InGame.ogg");
			_musicSource = World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
			_musicSource->SetRepeat(true);
		}
		if(i == 3)
		{
			RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/music_menu.ogg");
			_musicSource = World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
			_musicSource->SetRepeat(true);
		}
		if(i == 4)
		{
			RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/music_ende.ogg");
			_musicSource = World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
			_musicSource = nullptr;
		}
		if(i == 5)
		{
			RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/game_won.ogg");
			_musicSource = World::GetActiveWorld()->Downcast<World>()->GetAudioWorld()->PlaySound(audio);
			_musicSource = nullptr;
		}
	}
	
	void World::PlayOutro(bool win)
	{
		_inIntro = true;
		_console->Deactivate();
		
		Outro *outro = new Outro();
		outro->Play([&]{ Application::GetSharedInstance()->Downcast<Application>()->RecreateWorld(); }, win);
	}
	
	void World::LeftFromIntro()
	{
		if(!_inIntro)
			return;
		
		_inIntro = false;
		
		PlayMusic(3);
		RN::AudioResource *audio = RN::AudioResource::WithFile("Sounds/ambient_sound1.ogg");
		RN::openal::AudioSource *ambient = _audioWorld->PlaySound(audio);
		ambient->SetRepeat(true);
		
		RN::Vector2 resolution = RN::Window::GetSharedInstance()->GetSize();
		float aspect = resolution.y/resolution.x;
		float frustomHeight = 1920*aspect;
		_orthogonalSize.x = -frustomHeight*0.5f;
		_orthogonalSize.y = frustomHeight*0.5f;
		_orthogonalSize.z = -960.0f;
		_orthogonalSize.w = 960.0f;
		_camera->SetOrthogonalFrustum(_orthogonalSize.x, _orthogonalSize.y, _orthogonalSize.z, _orthogonalSize.w);
		_camera->SetClearColor(RN::Color::Black());
		_camera->SetClipFar(20000);
		
		RN::Billboard *background = new RN::Billboard();
		background->SetTexture(RN::Texture::WithFile("Textures/bg4.png"), 1.0f);
		background->SetPosition(RN::Vector3(0.0f, 0.0f, -10000.0f));
		background->SetSize(RN::Vector2(1920.0f, 1200.0f));
		background->GetMaterial()->SetLighting(false);
		background->GetMaterial()->SetAmbientColor(RN::Color::White());
		
		RN::Billboard *innerhole = new RN::Billboard();
		innerhole->SetTexture(RN::Texture::WithFile("Textures/inner_hole.png"), 1.0f);
		innerhole->SetPosition(RN::Vector3(-738.0f, -26.0f, -15000.0f));
		innerhole->GetMaterial()->SetLighting(false);
		
		AnimatableEntity *miniScreen = new AnimatableEntity();
		miniScreen->SetDefaultTexture(RN::Texture::WithFile("Textures/mini_screen+0.png"));
		miniScreen->PlayAnimationFile("Animations/mini_screen1.json");
		miniScreen->RepeateAnimation();
		miniScreen->SetPosition(RN::Vector3(0.0f, 0.0f, -9000.0f));
		
		RN::Billboard *medsglow = new RN::Billboard();
		medsglow->SetTexture(RN::Texture::WithFile("Textures/meds_blinking_glow.png"), 1.0f);
		medsglow->SetPosition(RN::Vector3(-738.0f, -397.0f, -9000.0f));
		medsglow->GetMaterial()->SetLighting(false);
		medsglow->GetMaterial()->SetAmbientColor(RN::Color::White());
		RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(1000), [medsglow]{
			medsglow->SetPosition(RN::Vector3(medsglow->GetPosition().x, medsglow->GetPosition().y, -9000.0f));
			
			RN::Timer::ScheduledTimerWithDuration(std::chrono::milliseconds(500), [medsglow]{medsglow->SetPosition(RN::Vector3(medsglow->GetPosition().x, medsglow->GetPosition().y, -11000.0f));}, false);
		}, true);
		
		Player::GetSharedInstance()->Reset();
		ProgressDoor::GetSharedInstance()->Reset();
		
		_console = new HackingConsole();
		_console->Activate();
	}
	
	void World::GameEnded()
	{
		_ended = true;
		_enemies->Enumerate<RN::SceneNode>([](RN::SceneNode *node, size_t index, bool &stop) {
			node->RemoveFromWorld();
		});
	}
	
	void World::Screenshake(float time, float strength)
	{
		_shakeTime = time;
		_shakeStrength = strength;
	}
	
	void World::Update(float delta)
	{
		if(_inIntro || _ended)
			return;
		
		if(!_console->IsHacking())
			return;
		
		_console->Update(delta);
		
		ProgressDoor::GetSharedInstance()->Progress(-0.5 * delta);
		
		_time -= delta;
		if(_time < 0.0f)
		{
			if(!_gamepad)
				_time = _rng->GetRandomFloatRange(2.0f, 5.0f);
			else
				_time = _rng->GetRandomFloatRange(1.0f, 2.0f);
			
			if(_console->CanSpawn())
			{
				Enemy *enemy = new Enemy();
				float height = _rng->GetRandomFloatRange(180.0f, 330.0f);
				enemy->SetPosition(RN::Vector3(1100.0f*((_rng->GetRandomInt32Range(0.0f, 2.0f) >= 1.0f)?-1.0f:1.0f), height, (height < 255)?-5000.0f:-2000.0f));
				
				if(_gamepad)
					enemy->GenerateQTE();
				
				if(enemy->GetPosition().x > 0.0f)
					enemy->SetSize(enemy->GetSize()*RN::Vector2(-1.0f, 1.0f));
				
				_enemies->AddObject(enemy->Autorelease());
			}
		}
		
		RN::Array *deleted = _enemies->GetObjectsPassingTest<Enemy>([](Enemy *enemy, bool &stop) -> bool {
			return (enemy->IsActive() == false);
		});
		
		deleted->Enumerate<Enemy>([&](Enemy *enemy, size_t index, bool &stop) {
			_enemies->RemoveObject(enemy);
		});
		
		if(_gamepad)
		{
			std::vector<int> pressed;
			
#define CheckButton(n) \
		do{ \
			if(!_gamepad->IsButtonPressed(n)) \
				_pressed &= ~(1 << n); \
			\
			if(_gamepad->IsButtonPressed(n) && !(_pressed & (1 << n))) \
			{ \
				_pressed |= (1 << n); \
				pressed.push_back(n); \
			} \
		} while(0)
			
			CheckButton(4);
			CheckButton(5);
			CheckButton(6);
			CheckButton(7);
			
			if(pressed.size() > 0)
			{
				bool missed = true;
				
				_enemies->Enumerate<Enemy>([&](Enemy *enemy, size_t index, bool &stop) {
					
					if(!enemy->IsActive())
						return;
					
					for(int button : pressed)
					{
						if(!enemy->ConsumeButton(button))
							return;
						
						missed = false;
					}
					
				});
				
				if(missed)
				{
					ProgressDoor::GetSharedInstance()->Progress(-2.5f);
					Screenshake(0.05, 0.1);
				}
			}
		}
		
		_shakeTime -= delta;
		if(_shakeTime > 0)
		{
			
			
#ifndef RN_PLATFORM_WINDOWS
			if(_gamepad)
				_gamepad->ExecuteCommand(RNCSTR("rumble"), RN::Number::WithUint8(255));
#endif
		}
		else
		{
			
			
#ifndef RN_PLATFORM_WINDOWS
			if(_gamepad)
				_gamepad->ExecuteCommand(RNCSTR("rumble"), RN::Number::WithUint8(0));
#endif
		}
	}
}
