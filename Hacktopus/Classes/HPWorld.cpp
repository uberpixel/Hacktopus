//
//  HPWorld.cpp
//  Hacktopus
//
//  Created by Sidney Just on 14/11/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "HPWorld.h"

namespace HP
{
	World::World() :
		RN::World("GenericSceneManager"),
		_gamepad(nullptr),
		_enemies(new RN::Array()),
		_time(0),
		_pressed(0)
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
	{}
	
	void World::LoadOnThread(RN::Thread *thread, RN::Deserializer *deserializer)
	{
		RN::openal::AudioListener *audioListener = new RN::openal::AudioListener();
		
		_camera = new RN::Camera(RN::Vector2(), RN::Texture::Format::RGB16F, (RN::Camera::Flags::Defaults | RN::Camera::Flags::Orthogonal) & ~RN::Camera::Flags::UseFog);
		_camera->AddAttachment(audioListener);
	}
	
	void World::LeftFromIntro()
	{
		_inIntro = false;
		
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
		background->SetTexture(RN::Texture::WithFile("Textures/background.png"), 1.0f);
		background->SetPosition(RN::Vector3(0.0f, 0.0f, -10000.0f));
		background->SetSize(RN::Vector2(1920.0f, 1200.0f));
		background->GetMaterial()->SetLighting(false);
		background->GetMaterial()->SetAmbientColor(RN::Color::White());
		
		AnimatableEntity *miniScreen = new AnimatableEntity();
		miniScreen->SetDefaultTexture(RN::Texture::WithFile("Textures/mini_screen+0.png"));
		miniScreen->PlayAnimationFile("Animations/mini_screen1.json");
		miniScreen->RepeateAnimation();
		miniScreen->SetPosition(RN::Vector3(0.0f, 0.0f, -9000.0f));
		
		RN::Camera *uiCamera = RN::UI::Server::GetSharedInstance()->GetCamera();
		uiCamera->SetFlags(uiCamera->GetFlags()|RN::Camera::Flags::NoFlush);
		RN::Texture *uiTexture = uiCamera->GetRenderTarget();
		RN::Billboard *uiBillboard = new RN::Billboard(false);
		uiBillboard->SetTexture(uiTexture, 1.0f);
		uiBillboard->SetPosition(RN::Vector3(0.0f, 0.0f, -9000));
		uiBillboard->SetScale(RN::Vector3(-1.0f, -1.0f, 1.0f));
		uiBillboard->GetMaterial()->SetLighting(false);
		uiBillboard->GetMaterial()->SetCullMode(RN::Material::CullMode::None);
		_camera->SetPriority(10000);
		
		Player::GetSharedInstance();
		ProgressDoor::GetSharedInstance();
		
		_console = new HackingConsole();
		_console->Activate();
	}
	
	void World::Screenshake(float time, float strength)
	{
		_shakeTime = time;
		_shakeStrength = strength;
	}
	
	void World::Update(float delta)
	{
		if(_inIntro)
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
			
			Enemy *enemy = new Enemy();
			float height = _rng->GetRandomFloatRange(180.0f, 330.0f);
			enemy->SetPosition(RN::Vector3(1100.0f*((_rng->GetRandomInt32Range(0.0f, 2.0f) >= 1.0f)?-1.0f:1.0f), height, (height < 255)?-5000.0f:-2000.0f));
			
			if(_gamepad)
				enemy->GenerateQTE();
			
			if(enemy->GetPosition().x > 0.0f)
				enemy->SetSize(enemy->GetSize()*RN::Vector2(-1.0f, 1.0f));
			
			_enemies->AddObject(enemy->Autorelease());
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
			float shake = _rng->GetRandomFloatRange(1.0-_shakeStrength, 1.0f);
			_camera->SetOrthogonalFrustum(_orthogonalSize.x*shake, _orthogonalSize.y*shake, _orthogonalSize.z*shake, _orthogonalSize.w*shake);
			
			if(_gamepad)
				_gamepad->ExecuteCommand(RNCSTR("rumble"), RN::Number::WithUint8(255));
		}
		else
		{
			_camera->SetOrthogonalFrustum(_orthogonalSize.x, _orthogonalSize.y, _orthogonalSize.z, _orthogonalSize.w);
			
			if(_gamepad)
				_gamepad->ExecuteCommand(RNCSTR("rumble"), RN::Number::WithUint8(0));
		}
	}
}
