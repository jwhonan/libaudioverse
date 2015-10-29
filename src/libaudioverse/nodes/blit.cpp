/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/
#include <libaudioverse/libaudioverse.h>
#include <libaudioverse/libaudioverse_properties.h>
#include <libaudioverse/nodes/blit.hpp>
#include <libaudioverse/private/node.hpp>
#include <libaudioverse/private/simulation.hpp>
#include <libaudioverse/private/properties.hpp>
#include <libaudioverse/private/macros.hpp>
#include <libaudioverse/private/memory.hpp>
#include <libaudioverse/private/constants.hpp>
#include <libaudioverse/implementations/blit.hpp>

namespace libaudioverse_implementation {

BlitNode::BlitNode(std::shared_ptr<Simulation> simulation): Node(Lav_OBJTYPE_BLIT_NODE, simulation, 0, 1), oscillator(simulation->getSr()) {
	appendOutputConnection(0, 1);
}

std::shared_ptr<Node> createBlitNode(std::shared_ptr<Simulation> simulation) {
	return standardNodeCreation<BlitNode>(simulation);
}

void BlitNode::process() {
	if(werePropertiesModified(this, Lav_BLIT_PHASE)) {
		oscillator.setPhase(getProperty(Lav_BLIT_PHASE).getFloatValue());
	}
	if(werePropertiesModified(this, Lav_BLIT_HARMONICS)) oscillator.setHarmonics(getProperty(Lav_BLIT_HARMONICS).getIntValue());
	if(werePropertiesModified(this, Lav_BLIT_FREQUENCY)) oscillator.setFrequency(getProperty(Lav_BLIT_FREQUENCY).getFloatValue());
	if(werePropertiesModified(this, Lav_BLIT_SHOULD_NORMALIZE)) oscillator.setShouldNormalize(getProperty(Lav_BLIT_SHOULD_NORMALIZE).getIntValue() == 1);
	auto &freq = getProperty(Lav_BLIT_FREQUENCY);
	if(freq.needsARate()) {
		for(int i = 0; i < block_size; i++) {
			oscillator.setFrequency(freq.getFloatValue(i));
			output_buffers[0][i] = oscillator.tick();
		}
	}
	else {
		for(int i = 0; i < block_size; i++) output_buffers[0][i] = oscillator.tick();
	}
}

void BlitNode::reset() {
	oscillator.reset();
}

//begin public api

Lav_PUBLIC_FUNCTION LavError Lav_createBlitNode(LavHandle simulationHandle, LavHandle* destination) {
	PUB_BEGIN
	auto simulation = incomingObject<Simulation>(simulationHandle);
	LOCK(*simulation);
	auto retval = createBlitNode(simulation);
	*destination = outgoingObject<Node>(retval);
	PUB_END
}

}