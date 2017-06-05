#include "AudioHandler.h"
#pragma warning (disable : 4996) //fopen

//---------------------STATIC VARIABLES---------------------

float AudioHandler::timeScale = 1.0f;

//Contexz and Device
ALCdevice*  AudioHandler::device = nullptr;
ALCcontext* AudioHandler::context = nullptr;

//Buffer(s)
ALuint AudioHandler::buffer[NUM_RPM];

//Sound File
unsigned char* AudioHandler::buf;   //Buffer with SoundFile
DWORD  AudioHandler::dataSize;		//Data size
ALuint AudioHandler::frequency;		//The Sample Rate of the WAVE file
ALenum AudioHandler::format;		//The audio format (bits per sample, number of channels)

//-----------------------CONSTRUCTORS------------------------

AudioHandler::AudioHandler()
{
	//Generate OpenAL Source(s) and link to "source"
	alGenSources(NUM_RPM*NUM_PROP, source);

	for (int p = 0; p < NUM_PROP; p++)
	{
		for (int r = 0; r < NUM_RPM; r++)
		{
			alSourcei(source[4*r+p], AL_BUFFER,	buffer[r]);         //Link the buffer to the source
			alSourcef(source[4*r+p], AL_PITCH,	1.0f);              //Set the pitch of the source
			alSourcef(source[4*r+p], AL_GAIN,	0.0f);              //Set the gain of the source
			alSourcei(source[4*r+p], AL_LOOPING, AL_TRUE);         //Set if source is looping sound
		}
		setSourcePosVel(p, Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f)); //Positions
	}
}

AudioHandler::~AudioHandler()
{
	alDeleteSources(NUM_PROP*NUM_RPM, source);       //Delete the OpenAL Source
}

//Static

void AudioHandler::error(const char* c)
{
	return; //INDEV
	std::cerr << c << std::endl;
}

void AudioHandler::initialize()
{
	//OpenAL needs to be initialized 
	device = alcOpenDevice(NULL);                      //Open the device
	if (!device) error("no sound device");      //Error during device oening
	context = alcCreateContext(device, NULL);          //Give the device a context
	alcMakeContextCurrent(context);                    //Make the context the current
	if (!context) error("no sound context");

	//freopen("LeFixDrone_Error.log", "w", stderr);
}

void AudioHandler::destroy()
{
	delete[] buf;

	alDeleteBuffers(NUM_RPM, buffer);                //Delete the OpenAL Buffer
	alcMakeContextCurrent(NULL);                //Make no context current
	alcDestroyContext(context);                 //Destroy the OpenAL Context
	alcCloseDevice(device);
}

void AudioHandler::loadFile(const char* c)
{
	//Replace existings sound file
	if (buf != nullptr) {
		delete[] buf;
		buf = nullptr;
	}

	//Loading of the WAVE file
	FILE *fp = nullptr;											//Create FILE pointer for the WAVE file
	fp = fopen(c, "rb");							//Open the WAVE file
	if (!fp) error("Failed to open file");	//Could not open file

	//Temp variables to store info about the WAVE file
	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;

	//Check that the WAVE file is OK
	fread(type, sizeof(char), 4, fp);                                              //Reads the first bytes in the file
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')      //Should be "RIFF"
		error("No RIFF");															//Not RIFF

	fread(&size, sizeof(DWORD), 1, fp);                                           //Continue to read the file
	fread(type, sizeof(char), 4, fp);                                             //Continue to read the file
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')     //Should be "WAVE"
		error("not WAVE");															//Not WAVE

	fread(type, sizeof(char), 4, fp);                                              //Continue to read the file
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')      //Should be "fmt "
		error("not fmt ");															//Not fmt 

	//Now we know that the file is a acceptable WAVE file
	//Info about the WAVE data is now read and stored
	fread(&chunkSize, sizeof(DWORD), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(DWORD), 1, fp);
	fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')       //This part should be "data"
		error("Missing DATA");															//not data

	fread(&dataSize, sizeof(DWORD), 1, fp);                                        //The size of the sound data is read

																				   //Display the info about the WAVE file
	std::cout << "Chunk Size: " << chunkSize << "\n";
	std::cout << "Format Type: " << formatType << "\n";
	std::cout << "Channels: " << channels << "\n";
	std::cout << "Sample Rate: " << sampleRate << "\n";
	std::cout << "Average Bytes Per Second: " << avgBytesPerSec << "\n";
	std::cout << "Bytes Per Sample: " << bytesPerSample << "\n";
	std::cout << "Bits Per Sample: " << bitsPerSample << "\n";
	std::cout << "Data Size: " << dataSize << "\n";

	buf = new unsigned char[dataSize];                            //Allocate memory for the sound data

	
	
	error((std::to_string(fread(buf, sizeof(BYTE), dataSize, fp)) + " bytes loaded\n").c_str());

	fclose(fp);

	//Set openAl values
	frequency = sampleRate;

	switch (bitsPerSample+channels)
	{
	case 9:  format = AL_FORMAT_MONO8;		break;
	case 10: format = AL_FORMAT_STEREO8;	break;
	case 17: format = AL_FORMAT_MONO16;		break;
	case 18: format = AL_FORMAT_STEREO16;	break;
	default:  error("Wrong BitPerSample"); //Not valid format
	}
}

void AudioHandler::createBuffer(eRPM rpm)
{
	alGenBuffers(1, &buffer[rpm]);									//Generate one OpenAL Buffer and link to "buffer"
	alBufferData(buffer[rpm], format, buf, dataSize, frequency);		//Store the sound data in the OpenAL Buffer
	if (alGetError() != AL_NO_ERROR)
		error("Error loading ALBuffer");
}

void AudioHandler::setListener(const Vector3f& pos, const Vector3f& vel, const Quaternionf& rot)
{
	Matrix3f rotMat = rot.toRotationMatrix();
	Vector3f dir = rotMat * Vector3f(0.0f, 1.0f, 0.0f);
	Vector3f up  = rotMat * Vector3f(0.0f, 0.0f, 1.0f);

	//float ListenerPos[3];	//Position of the listener
	//float ListenerVel[3];	//Velocity of the listener
	float ListenerOri[6];	//Orientation of the listener
							//First direction vector, then vector pointing up) 

	//pos.getVariables(ListenerPos[0], ListenerPos[1], ListenerPos[2]);
	//vel.getVariables(ListenerVel[0], ListenerVel[1], ListenerVel[2]);
	ListenerOri[0] = dir.x();
	ListenerOri[1] = dir.y();
	ListenerOri[2] = dir.z();
	ListenerOri[3] = up.x();
	ListenerOri[4] = up.y();
	ListenerOri[5] = up.z();

	//Listener                                                                               
	alListenerfv(AL_POSITION, pos.data());                     //Set position of the listener
	alListenerfv(AL_VELOCITY, vel.data());                     //Set velocity of the listener
	alListenerfv(AL_ORIENTATION, ListenerOri);                  //Set orientation of the listener
}

void AudioHandler::setTimeScale(float &_timeScale)
{
	timeScale = _timeScale;
}

//Member (Drone)

void AudioHandler::setSourceSound(int prop, float speed)
{
	for (int r = 0; r < NUM_RPM; r++)
	{
		float gain, pitch;
		switch (r)
		{
		//pitch difference ca 1.7f bzw. 0.59f
		case mid:
			gain = 1.0f - abs(0.4f - speed) / 0.4f; //0 bei 0%, 1 bei 40%, 0 bei 80%
			pitch = 1.0f - (0.4f - speed) * 1.75f; //1.0f bei 40% 1.7 bei 80%
			break;

		case hgh:
			gain = 1.0f - (0.8f - speed) / 0.4f; //0 bei 40%, 1 bei 80%
			if (gain > 1.0f) gain = 1.0f;
			pitch = 1.0f - (0.8f - speed) * 1.0f; //0.6f bei 40%, 1.0f bei 80%,
			break;
		}
		pitch *= timeScale;
		if (gain < 0.0f) gain = 0.0f;
		gain *= (1.0f - (speed - 1.0f)*(speed - 1.0f)); //Global volume curve

		gain *= Settings::audioVolume.get(); //Global volume setting

		if (Settings::camMode.get() == camModeD1 || Settings::camMode.get() == camModeD3) gain *= 0.2f; //Cam drone too close, turn volum down


		alSourcef(source[4*r+prop], AL_PITCH, pitch);
		alSourcef(source[4*r+prop], AL_GAIN,  gain);
	}
}

void AudioHandler::setSourcePosVel(int prop, Vector3f pos, Vector3f vel)
{
	for (int r = 0; r < NUM_RPM; r++)
	{
		alSourcefv(source[4*r+prop], AL_POSITION, pos.data());      //Set the position of the source
		alSourcefv(source[4*r+prop], AL_VELOCITY, vel.data());      //Set the velocity of the source
	}
}

void AudioHandler::playSources()
{
	for (int n = 0; n < NUM_PROP*NUM_RPM; n++)
	{
		alSourcePlay(source[n]);
		if (alGetError() != AL_NO_ERROR)
			error("Error playing sound");
	}
}

void AudioHandler::stopSources()
{
	for (int n = 0; n < NUM_PROP*NUM_RPM; n++)
	{
		alSourceStop(source[n]);
		if (alGetError() != AL_NO_ERROR)
			error("Error playing sound");
	}
}