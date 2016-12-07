/* Copyright 2016 Libaudioverse Developers. See the COPYRIGHT
file at the top-level directory of this distribution.

Licensed under the mozilla Public License, version 2.0 <LICENSE.MPL2 or
https://www.mozilla.org/en-US/MPL/2.0/> or the Gbnu General Public License, V3 or later
<LICENSE.GPL3 or http://www.gnu.org/licenses/>, at your option. All files in the project
carrying such notice may not be copied, modified, or distributed except according to those terms. */
#pragma once
#include "../private/node.hpp"
#include <memory>

namespace libaudioverse_implementation {

class Server;
class CrossfadingDelayLine;

class CrossfadingDelayNode: public Node {
	public:
	CrossfadingDelayNode(std::shared_ptr<Server> server, float maxDelay, int channels);
	~CrossfadingDelayNode();
	void process();
	protected:
	void delayChanged();
	void recomputeDelta();
	unsigned int delay_line_length = 0;
	CrossfadingDelayLine **lines;
	int channels;
};

std::shared_ptr<Node> createCrossfadingDelayNode(std::shared_ptr<Server> server, float maxDelay, unsigned int channels);
}