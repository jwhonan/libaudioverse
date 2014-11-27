/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/
#pragma once
#include "libaudioverse.h"
#include "private_simulation.hpp"
#include <vector>
#include <set>
#include <memory>
#include <utility>
#include <atomic>
#include <mutex>
#include <thread>

/**A physical output.*/
class LavDevice {
	protected:
	LavDevice(std::shared_ptr<LavSimulation> simulation, unsigned int mixAhead);
	virtual ~LavDevice();
	virtual void init(unsigned int targetSr, unsigned int userRequestedChannels, unsigned int channels); //second step in initialization. We can't just fall through to the constructor.
	virtual void start(); //final step in initialization via subclasses: starts the background thread.
	virtual void stop(); //stop the output.
	//these hooks are run in the background thread, and should be overridden in subclasses.
	virtual void startup_hook();
	virtual void shutdown_hook();
	virtual void zeroOrNextBuffer(float* where);
	virtual void mixingThreadFunction();
	unsigned int channels = 0, user_requested_channels = 0;
	unsigned int mix_ahead = 0;
	unsigned int input_buffer_size, output_buffer_size, input_buffer_frames, output_buffer_frames;
	float* mixing_matrix = nullptr;
	bool should_apply_mixing_matrix = false;
	unsigned int next_output_buffer = 0;
	unsigned int callback_buffer_index = 0;
	unsigned int target_sr = 0;
	float** buffers = nullptr;
	std::atomic<int>* buffer_statuses = nullptr;
	std::atomic_flag mixing_thread_continue;
	std::shared_ptr<LavSimulation> simulation = nullptr;
	std::thread mixing_thread;
	bool started = false;
};

class LavSimulationFactory {
	public:
	LavSimulationFactory() = default;
	virtual ~LavSimulationFactory() {}
	virtual std::vector<std::string> getOutputNames() = 0;
	//returns -1.0f for unknown.
	virtual std::vector<float> getOutputLatencies() = 0;
	virtual std::vector<int> getOutputMaxChannels() = 0;
	//if useDefaults is on, the last three parameters don't matter.
	//useDefaults is a request to the backend to do something appropriate.
	virtual std::shared_ptr<LavSimulation> createSimulation(int index, bool useDefaults, unsigned int channels, unsigned int sr, unsigned int blockSize, unsigned int mixAhead) = 0;
	virtual unsigned int getOutputCount();
	virtual std::string getName();
	protected:
	int output_count = 0;
};

typedef LavSimulationFactory* (*LavSimulationFactoryCreationFunction)();
LavSimulationFactory* createWinmmSimulationFactory();
LavSimulationFactory* createOpenALSimulationFactory();


//finally, the function that initializes all of this.
void initializeSimulationFactory();

