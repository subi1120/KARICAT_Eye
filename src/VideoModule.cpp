#include "VideoModule.h"


VideoModule::VideoModule()
{
}


VideoModule::~VideoModule()
{
}

void VideoModule::registerFrameCallback(VideoModule::FrameCallback* frameCallback, int type)
{
	_frameCallback = frameCallback;
}
