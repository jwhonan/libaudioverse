/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/
#pragma once
#include "../libaudioverse.h"
#include "environmentbase.hpp"
#include <vector>
#include <set>
#include <memory>

class LavSourceNode;
class LavHrtfData;

class LavSimpleEnvironmentNode: public LavEnvironmentBase {
	public:
	LavSimpleEnvironmentNode(std::shared_ptr<LavSimulation> simulation, std::shared_ptr<LavHrtfData> hrtf);
	void registerSourceForUpdates(std::shared_ptr<LavSourceNode> source);
	//call update on all sources.
	virtual void willProcessParents();
	std::shared_ptr<LavNode> createPannerNode();
	private:
	//while these may be parents (through virtue of the panners we give out), they also have to hold a reference to us-and that reference must be strong.
	//the world is more capable of handling a source that dies than a source a world that dies.
	std::set<std::weak_ptr<LavSourceNode>, std::owner_less<std::weak_ptr<LavSourceNode>>> sources;
	std::shared_ptr<LavHrtfData > hrtf;
	std::shared_ptr<LavNode> output=nullptr;
	LavEnvironment environment;
};