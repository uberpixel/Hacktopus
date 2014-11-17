//
//  HPProgressDoor.h
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
