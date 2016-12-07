/* Copyright 2016 Libaudioverse Developers. See the COPYRIGHT
file at the top-level directory of this distribution.

Licensed under the mozilla Public License, version 2.0 <LICENSE.MPL2 or
https://www.mozilla.org/en-US/MPL/2.0/> or the Gbnu General Public License, V3 or later
<LICENSE.GPL3 or http://www.gnu.org/licenses/>, at your option. All files in the project
carrying such notice may not be copied, modified, or distributed except according to those terms. */
#include <libaudioverse/libaudioverse.h>
#include <libaudioverse/libaudioverse_properties.h>
#include <libaudioverse/nodes/pull.hpp>
#include <libaudioverse/private/server.hpp>
#include <libaudioverse/private/node.hpp>
#include <libaudioverse/private/properties.hpp>
#include <libaudioverse/private/macros.hpp>
#include <libaudioverse/private/memory.hpp>
#include <speex_resampler_cpp.hpp>
#include <memory>

namespace libaudioverse_implementation {

PullNode::PullNode(std::shared_ptr<Server> s, unsigned int inputSr, unsigned int channels): Node(Lav_OBJTYPE_PULL_NODE, s, 0, channels) {
	this->channels = channels;
	input_sr = inputSr;
	resampler = speex_resampler_cpp::createResampler(s->getBlockSize(), channels, inputSr, (int)s->getSr());
	this->channels = channels;
	incoming_buffer = allocArray<float>(channels*server->getBlockSize());
	resampled_buffer = allocArray<float>(channels*s->getBlockSize());
	appendOutputConnection(0, channels);
}

std::shared_ptr<Node> createPullNode(std::shared_ptr<Server> server, unsigned int inputSr, unsigned int channels) {
	return standardNodeCreation<PullNode>(server, inputSr, channels);
}

PullNode::~PullNode() {
	freeArray(incoming_buffer);
	freeArray(resampled_buffer);
}

void PullNode::process() {
	//first get audio into the resampler if needed.
	int got = 0;
	while(got < block_size) {
		got += resampler->write(resampled_buffer, block_size-got);
		if(got >= block_size) break; //we may have done it on this iteration.
		if(callback) {
			callback(outgoingObject(this->shared_from_this()), block_size, channels, incoming_buffer, callback_userdata);
		} else {
			memset(incoming_buffer, 0, block_size*sizeof(float)*channels);
		}
		resampler->read(incoming_buffer);
	}
	//this is simply uninterweaving, but taking advantage of the fact that we have a different output destination.
	for(unsigned int i = 0; i < block_size*channels; i+=channels) {
		for(unsigned int j = 0; j < channels; j++) {
			output_buffers[j][i/channels] = resampled_buffer[i+j];
		}
	}
}

//begin public api.

Lav_PUBLIC_FUNCTION LavError Lav_createPullNode(LavHandle serverHandle, unsigned int sr, unsigned int channels, LavHandle* destination) {
	PUB_BEGIN
	auto server = incomingObject<Server>(serverHandle);
	LOCK(*server);
	*destination = outgoingObject<Node>(createPullNode(server, sr, channels));
	PUB_END
}

Lav_PUBLIC_FUNCTION LavError Lav_pullNodeSetAudioCallback(LavHandle nodeHandle, LavPullNodeAudioCallback callback, void* userdata) {
	PUB_BEGIN
	auto node = incomingObject<Node>(nodeHandle);
	LOCK(*node);
	if(node->getType() != Lav_OBJTYPE_PULL_NODE) ERROR(Lav_ERROR_TYPE_MISMATCH, "Expected a pull node.");
	auto p = std::static_pointer_cast<PullNode>(node);
	p->callback = callback;
	p->callback_userdata = userdata;
	PUB_END
}

}