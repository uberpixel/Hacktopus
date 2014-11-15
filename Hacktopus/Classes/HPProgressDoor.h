//
//  HPProgressDoor.h
//  Hacktopus
//
//  Created by Nils Daumann on 15.11.14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#ifndef __Hacktopus__HPProgressDoor__
#define __Hacktopus__HPProgressDoor__

#include <Rayne/Rayne.h>

namespace HP
{
	class ProgressDoor : public RN::ISingleton<ProgressDoor>
	{
	public:
		ProgressDoor();
		~ProgressDoor();
		
		void Progress(float progress);
		float GetProgress() const { return _progress; }
		void Reset();
		
	private:
		float _progress;
		bool _isOpened;
		RN::Vector3 _position;
		RN::Vector3 _offsetFactor;
		RN::Billboard *_topDoor;
		RN::Billboard *_bottomDoor;
		
		RNDeclareSingleton(ProgressDoor)
	};
}

#endif /* defined(__Hacktopus__HPProgressDoor__) */
