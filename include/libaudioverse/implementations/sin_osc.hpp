/* Copyright 2016 Libaudioverse Developers. See the COPYRIGHT
file at the top-level directory of this distribution.

Licensed under the mozilla Public License, version 2.0 <LICENSE.MPL2 or
https://www.mozilla.org/en-US/MPL/2.0/> or the Gbnu General Public License, V3 or later
<LICENSE.GPL3 or http://www.gnu.org/licenses/>, at your option. All files in the project
carrying such notice may not be copied, modified, or distributed except according to those terms. */
#pragma once
#include <math.h>
#include "../private/constants.hpp"

namespace libaudioverse_implementation {

/*A fast sine oscillator.

The following algorithm is based off the angle sum and difference identities.
This is also equivalent to rotating a unit vector by f/sr radians every tick.

Trig identities:
sin(x+d)=sin(x)cos(d)+cos(x)sin(d)
cos(x+d)=cos(x)cos(d)-sin(x)sin(d)	

We have to use double, or the oscillator becomes erronious within only a few samples.

Resync is the number of samples after which we resynchronize the oscillator; 64 is usually sufficient.
*/

class SinOsc {
	public:
	SinOsc(float _sr, int _resync = 100): sr(_sr), resync(_resync), resyncCounter(_resync) {}
	
	double tick() {
		if(phase > phaseWrap) {
			phase -= floor(phase/phaseWrap)*phaseWrap;
			if(phaseWrap != 1.0) doResync();
		}
		resyncCounter --;
		if(resyncCounter == 0) doResync();
		double ocx=cx, osx=sx;
		sx = osx*cd+ocx*sd;
		cx = ocx*cd-osx*sd;
		phase += phaseIncrement;
		//Return the old one, so that we start at phase zero properly.
		return osx;
	}

	void doResync() {
		sx = sin(2*PI*phase);
		cx = cos(2*PI*phase);
		resyncCounter = resync;
	}
	
	//Skips count samples.
	//Same as calling tick count times.
	void skipSamples(int count) {
		phase += phaseIncrement*count;
		phase -= floor(phase/phaseWrap)*phaseWrap;
		doResync();
	}

	//Set the phase increment per sample.
	void setPhaseIncrement(double i) {
		phaseIncrement = i;
		cd = cos(phaseIncrement*2*PI);
		sd = sin(phaseIncrement*2*PI);
	}
	
	void setFrequency(float f) {
		frequency = f;
		setPhaseIncrement(f/sr);
	}

	void reset() {
		setPhase(0.0);
		doResync();
	}
	
	//phase is from 0 to 1 and measured in  periods.
	void setPhase(double p) {
		p -= floor(p);
		phase = p;
		doResync();
	}
	
	double getPhase() {
		return phase;
	}
	
	//Usually this is 1, but things like the blit need custom configurations.
	void setPhaseWrap(double p) {
		phaseWrap = p;
	}
	
	private:
	//s=sin, c=cos
	//internal vector is right, frequency is zero.
	double sx = 0, cx = 1, sd = 0, cd = 0;
	float sr; //sampling rate.
	//frequency is saved for purposes of skipping samples.
	double frequency =0;
	double phase = 0, phaseIncrement = 0, phaseWrap = 1.0;
	int resync, resyncCounter;
};

}