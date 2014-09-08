/**Copyright (C) Austin Hicks, 2014
This file is part of Libaudioverse, a library for 3D and environmental audio simulation, and is released under the terms of the Gnu General Public License Version 3 or (at your option) any later version.
A copy of the GPL, as well as other important copyright and licensing information, may be found in the file 'LICENSE' in the root of the Libaudioverse repository.  Should this file be missing or unavailable to you, see <http://www.gnu.org/licenses/>.*/
#include <libaudioverse/libaudioverse.h>
#include <libaudioverse/private_macros.hpp>
#include <libaudioverse/private_memory.hpp>
#include <libaudioverse/private_audio_devices.hpp>
#include <stdlib.h>
#include <functional>
#include <algorithm>
#include <iterator>
#include <thread>
#include <stdio.h>
#include <stdarg.h>
#include <libaudioverse/private_logging.hpp>

LavLogger *logger = nullptr;

LavLogger::LavLogger() {
	logging_thread = std::thread([this]() {loggingThreadFunction();});
	workspace = new char[workspace_length];
}

void LavLogger::log(int level, std::string fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	int got = vsnprintf(workspace, workspace_length, fmt.c_str(), argptr);
	va_end(argptr);
	if(got == 0) return;
	LavLogMessage msg(level, std::string(workspace), false);
	log_queue.enqueue(msg);
}

void LavLogger::setLoggingLevel(int level) {
	config_mutex.lock();
	this->level = level;
	config_mutex.unlock();
}

int LavLogger::getLoggingLevel() {
	config_mutex.lock();
	int retval = level;
	config_mutex.unlock();
	return retval;
}

void LavLogger::setLoggingCallback(LavLoggingCallback cb) {
	config_mutex.lock();
	callback = cb;
	config_mutex.unlock();
}

LavLoggingCallback LavLogger::getLoggingCallback() {
	config_mutex.lock();
	LavLoggingCallback retval = callback;
	config_mutex.unlock();
	return retval;
}

void LavLogger::loggingThreadFunction() {
	bool shouldContinue = true;
	while(shouldContinue) {
		LavLogMessage msg= log_queue.dequeue();
		config_mutex.lock();
		if(callback && msg.level >= level) callback(msg.level, msg.message.c_str(), msg.is_final);
		config_mutex.unlock();
		if(msg.is_final) shouldContinue = false;
	}
}

void log(int level, std::string fmt, ...) {
}

Lav_PUBLIC_FUNCTION LavError Lav_setLoggingCallback(LavLoggingCallback cb) {
	PUB_BEGIN
	PUB_END
}

Lav_PUBLIC_FUNCTION LavError Lav_getLoggingCallback(LavLoggingCallback* destination) {
	PUB_BEGIN
	PUB_END
}

Lav_PUBLIC_FUNCTION LavError Lav_setLoggingLevel(int level) {
	PUB_BEGIN
	PUB_END
}

Lav_PUBLIC_FUNCTION LavError Lav_getLoggingLevel(int* destination) {
	PUB_BEGIN
	PUB_END
}