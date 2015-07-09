#pragma once
#include <openni2/OpenNI.h>
#include "VideoModule.h"

using namespace openni;

class XtionProCamera :
	public VideoModule
{
private:
	class Callback : public VideoStream::NewFrameListener
	{
	public:
		void onNewFrame(VideoStream& stream);
		void registerFrameCallback(VideoModule::FrameCallback* callback)
		{
			_frameCallback = callback;
		}

	private:
		VideoFrameRef _frame;
		VideoModule::FrameCallback* _frameCallback;
	};




public:

	XtionProCamera();
	~XtionProCamera();

	virtual int initialize(int type, int frameWidth, int frameHeight);
	virtual int start(int type);
	virtual int stop(int type);
	virtual int pause(int type);
	virtual int resume(int type);
	virtual void registerFrameCallback(VideoModule::FrameCallback* frameCallback, int type);

	static int deviceCounter;

private:
	Device _device;

	VideoStream _colorStream;
	VideoStream _depthStream;

	Callback _depthCallback;
	Callback _colorCallback;


};

