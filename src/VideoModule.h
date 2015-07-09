#pragma once

#include <vector>
using namespace std;

class VideoModule
{
public:
	enum FrameType
	{
		eFrameType_Depth = 1,
		eFrameType_Gray = 2,
		eFrameType_RGB = 4,
	};

	class FrameCallback
	{
	public:
			
		FrameCallback(){}
		virtual ~FrameCallback(){}

		virtual void onNewFrame(char* pFrame, int width, int height, int type) = 0;
	};


	VideoModule();
	virtual ~VideoModule() = 0;

	virtual int initialize(int type, int frameWidth, int frameHeigt) = 0;
	virtual int start(int type) = 0;
	virtual int stop(int type) = 0;
	virtual int pause(int type) = 0;
	virtual int resume(int type) = 0;
	virtual void registerFrameCallback(VideoModule::FrameCallback* frameCallback, int type);

protected:

	FrameCallback* _frameCallback;


private:

};

