/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/
#pragma once
#include "libaudioverse.h"
#include "private_devices.hpp"
#include <vector>
#include <set>
#include <memory>
#include <utility>
#include <atomic>
#include <mutex>
#include <thread>

/**A physical output.*/
class LavPhysicalOutput {
	protected:
	LavPhysicalOutput(std::shared_ptr<LavDevice> dev, unsigned int mixAhead);
	virtual void init(unsigned int targetSr); //second step in initialization. We can't just fall through to the constructor.
	virtual void start(); //final step in initialization via subclasses: starts the background thread.
	virtual void stop(); //stop the output.
	virtual ~LavPhysicalOutput();
	virtual void zeroOrNextBuffer(float* where);
	virtual void mixingThreadFunction();
	unsigned int buffer_size = 0, mix_ahead = 0;
	unsigned int next_output_buffer = 0;
	float** buffers = nullptr;
	std::atomic<int>* buffer_statuses = nullptr;
	friend class LavPhysicalOutputFactory;
	std::atomic_flag background_thread_continue;
	std::mutex ensure_stopped_mutex; //held by the background thread as long as that thread is running.
	std::shared_ptr<LavDevice> device = nullptr;
	std::thread mixing_thread;
};

class LavPhysicalOutputFactory {
	public:
	LavPhysicalOutputFactory() = delete;
	virtual ~LavPhysicalOutputFactory() {}
	virtual std::vector<std::string> getOutputNames() = 0;
	virtual std::vector<float> getOutputLatencies() = 0;
	virtual std::vector<int> getOutputMaxChannels() = 0;
	virtual void rescan();
};

bool portaudioBackendAvailable();
LavPhysicalOutputFactory* createPortaudioPhysicalOutputFactory();