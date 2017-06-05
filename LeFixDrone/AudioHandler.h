#pragma once

#include <iostream>
#include <windows.h>

#include <AL/al.h>
#include <AL/alc.h>

#include "LeFixDrone.h"

using namespace LeFixDrone;
using namespace Eigen;

class AudioHandler
{
public:
	AudioHandler();
	~AudioHandler();

	static void initialize();
	static void destroy();

	static void loadFile(const char* c);
	static void setListener(const Vector3f& pos, const Vector3f& vel, const Quaternionf& rot);
	static void createBuffer(eRPM rpm);

	static void setTimeScale(float &_timeScale);

	ALuint source[NUM_RPM*NUM_PROP];

	void setSourceSound(int prop, float rpm);
	void setSourcePosVel(int prop, Vector3f pos, Vector3f vel);

	void playSources();
	void stopSources();

private:
	static float timeScale;

	//Context and Device
	static ALCdevice*  device;
	static ALCcontext* context;

	//Buffer(s)
	static ALuint buffer[NUM_RPM];

	//Sound File
	static unsigned char* buf;     //buffer with SoundFile
	static DWORD dataSize;
	static ALuint frequency;		//The Sample Rate of the WAVE file
	static ALenum format;			//The audio format (bits per sample, number of channels)

	static void error(const char* c);

};

