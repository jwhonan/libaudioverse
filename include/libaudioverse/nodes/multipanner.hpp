/* Copyright 2016 Libaudioverse Developers. See the COPYRIGHT
file at the top-level directory of this distribution.

Licensed under the mozilla Public License, version 2.0 <LICENSE.MPL2 or
https://www.mozilla.org/en-US/MPL/2.0/> or the Gbnu General Public License, V3 or later
<LICENSE.GPL3 or http://www.gnu.org/licenses/>, at your option. All files in the project
carrying such notice may not be copied, modified, or distributed except according to those terms. */
#pragma once
#include "../private/node.hpp"
#include "../implementations/multipanner.hpp"
#include <memory>

namespace libaudioverse_implementation {

class Server;
class HrtfData;

class MultipannerNode: public Node {
	public:
	MultipannerNode(std::shared_ptr<Server> server, std::shared_ptr<HrtfData> hrtf);
	void strategyChanged();
	void process() override;
	void reset() override;
	private:
	Multipanner panner;
};

std::shared_ptr<Node> createMultipannerNode(std::shared_ptr<Server> server, std::shared_ptr<HrtfData> hrtf);
}