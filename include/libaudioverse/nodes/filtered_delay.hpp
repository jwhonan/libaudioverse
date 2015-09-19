/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/
#pragma once
#include "../private/node.hpp"
#include <memory>

namespace libaudioverse_implementation {

class Simulation;
class CrossfadingDelayLine;
class BiquadFilter;

class FilteredDelayNode: public Node {
	public:
	FilteredDelayNode(std::shared_ptr<Simulation> simulation, float maxDelay, unsigned int channels);
	~FilteredDelayNode();
	void process();
	void reset() override;
	protected:
	void delayChanged();
	void recomputeDelta();
	void reconfigureBiquads();
	unsigned int delay_line_length = 0;
	CrossfadingDelayLine **lines;
	BiquadFilter** biquads;
	int prev_type; //used for biquad history resets.
	int channels;
};

std::shared_ptr<Node> createFilteredDelayNode(std::shared_ptr<Simulation> simulation, float maxDelay, unsigned int channels);
}