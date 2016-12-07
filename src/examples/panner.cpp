/* Copyright 2016 Libaudioverse Developers. See the COPYRIGHT
file at the top-level directory of this distribution.

Licensed under the mozilla Public License, version 2.0 <LICENSE.MPL2 or
https://www.mozilla.org/en-US/MPL/2.0/> or the Gbnu General Public License, V3 or later
<LICENSE.GPL3 or http://www.gnu.org/licenses/>, at your option. All files in the project
carrying such notice may not be copied, modified, or distributed except according to those terms. */

/**Demonstrates the hrtf node.*/
#include <libaudioverse/libaudioverse.h>
#include <libaudioverse/libaudioverse_properties.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ERRCHECK(x) do {\
if((x) != Lav_ERROR_NONE) {\
	printf(#x " errored: %i", (x));\
	Lav_shutdown();\
	return 1;\
}\
} while(0)\

int main(int argc, char** args) {
	if(argc != 3) {
		printf("Usage: %s <sound file> <channels>", args[0]);
		return 1;
	}
	int channels = 0;
	sscanf(args[2], "%d", &channels);
	if(channels != 2
&& channels != 6
&& channels != 8) {
		printf("Cannot work with %d channels", channels);
		return 1;
	}
	LavHandle server;
	LavHandle bufferNode, panNode, limit;
	ERRCHECK(Lav_initialize());
	ERRCHECK(Lav_createServer(44100, 1024, &server));
	ERRCHECK(Lav_serverSetOutputDevice(server, "default", channels));
	ERRCHECK(Lav_createBufferNode(server, &bufferNode));
	LavHandle buffer;
	ERRCHECK(Lav_createBuffer(server, &buffer));
	ERRCHECK(Lav_bufferLoadFromFile(buffer, args[1]));
	ERRCHECK(Lav_nodeSetBufferProperty(bufferNode, Lav_BUFFER_BUFFER, buffer));
	ERRCHECK(Lav_createMultipannerNode(server, "default", &panNode));
	int pantype =Lav_PANNING_STRATEGY_STEREO;
	switch(channels) {
		case 6: pantype =Lav_PANNING_STRATEGY_SURROUND51; break;
		case 8: pantype=Lav_PANNING_STRATEGY_SURROUND71; break;
	}
	ERRCHECK(Lav_nodeSetIntProperty(panNode, Lav_PANNER_STRATEGY, pantype));
	ERRCHECK(Lav_nodeConnect(bufferNode, 0, panNode, 0));
	ERRCHECK(Lav_createHardLimiterNode(server, channels, &limit));
	ERRCHECK(Lav_nodeConnect(panNode, 0, limit, 0));
	ERRCHECK(Lav_nodeConnectServer(limit, 0));
	int shouldContinue = 1;
	printf("Enter pairs of numbers separated by whitespace, where the first is azimuth (anything) and the second\n"
"is elevation (-90 to 90).\n"
"Input q to quit.\n");
	char command[512] = "";
	float elev = 0, az = 0;
	int elevOrAz = 0;
	while(shouldContinue) {
		scanf("%s", command);
		if(command[0] == 'q') {
			shouldContinue = 0;
			continue;
		}
		if(elevOrAz == 0) {
			sscanf(command, "%f", &az);
			elevOrAz = 1;
			continue;
		}
		else if(elevOrAz == 1) {
			sscanf(command, "%f", &elev);
			ERRCHECK(Lav_nodeSetFloatProperty(panNode, Lav_PANNER_ELEVATION, elev));
			ERRCHECK(Lav_nodeSetFloatProperty(panNode, Lav_PANNER_AZIMUTH, az));
			elevOrAz = 0;
			continue;
		}
	}
	Lav_shutdown();
	return 0;
}
