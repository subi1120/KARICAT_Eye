#include "XtionProCamera.h"

int XtionProCamera::deviceCounter = 0;

XtionProCamera::XtionProCamera()
{
	if (deviceCounter == 0)
	{
		Status res = OpenNI::initialize();
		if (res != STATUS_OK)
			{
				printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
			}


	}

	deviceCounter++;
}



XtionProCamera::~XtionProCamera()
{

	deviceCounter--;



	if (_device.isValid())
		_device.close();

	if (deviceCounter == 0)
	{
		OpenNI::shutdown();
	
	}

}

void XtionProCamera::Callback::onNewFrame(VideoStream& stream)
{
	stream.readFrame(&_frame);
	if (_frameCallback)
	{
		int format = _frame.getVideoMode().getPixelFormat();
		int type = 0;
		if (format == PIXEL_FORMAT_DEPTH_1_MM || format == PIXEL_FORMAT_DEPTH_100_UM)
		{
			type = eFrameType_Depth;
		}
		else if (format == PIXEL_FORMAT_RGB888)
		{
			type = eFrameType_RGB;
		}

		_frameCallback->onNewFrame((char*)_frame.getData(), _frame.getWidth(), _frame.getHeight(), type);
	}

}




int XtionProCamera::initialize(int type, int frameWidth, int frameHeight)
{
	int res = 0;

	Array<DeviceInfo> deviceArray;
	OpenNI::enumerateDevices(&deviceArray);

	if (deviceArray.getSize() == 0)
	{

		return -1;
	}

	res = _device.open(deviceArray[deviceCounter-1].getUri());




	if (res != STATUS_OK)
	{
		return -1;
	}

	if ((type & VideoModule::eFrameType_Depth) != 0)
	{
		res = _depthStream.create(_device, SENSOR_DEPTH);
		if (res != STATUS_OK)
			return -1;

		VideoMode depthMode = _depthStream.getVideoMode();
		depthMode.setResolution(frameWidth, frameHeight);
		res = _depthStream.setVideoMode(depthMode);
	}

	if ((type & VideoModule::eFrameType_RGB) != 0)
	{
		res = _colorStream.create(_device, SENSOR_COLOR); 
		if (res != STATUS_OK)
			return -1;
		
		VideoMode colorMode = _colorStream.getVideoMode();
		colorMode.setResolution(frameWidth, frameHeight);
		res = _colorStream.setVideoMode(colorMode);
		
	}

	_device.setDepthColorSyncEnabled(TRUE);
	return 0;
}

int XtionProCamera::start(int type)
{
	int res = 0;

	if ((type & VideoModule::eFrameType_Depth) != 0)
	{
		res = _depthStream.start();
		if (res != STATUS_OK)
			return -1;
	}

	if ((type & VideoModule::eFrameType_RGB) != 0)
	{
		res = _colorStream.start();
		if (res != STATUS_OK)
			return -1;
	}

	return 0;
}

int XtionProCamera::stop(int type)
{
	if (_depthStream.isValid())
	{
		_depthStream.removeNewFrameListener(&_depthCallback);
		_depthStream.stop();
		_depthStream.destroy();
	}
	
	if (_colorStream.isValid())
	{
		_colorStream.removeNewFrameListener(&_colorCallback);
		_colorStream.stop();
		_colorStream.destroy();
	}

	return 0;
}

int XtionProCamera::pause(int type)
{

	return 0;
}

int XtionProCamera::resume(int type)
{

	return 0;
}

void XtionProCamera::registerFrameCallback(VideoModule::FrameCallback* frameCallback, int type)
{
	//VideoModule::registerFrameCallback(frameCallback);

	if (type == VideoModule::eFrameType_Depth)
	{
		_depthCallback.registerFrameCallback(frameCallback);

		if (_depthStream.isValid())
			_depthStream.addNewFrameListener(&_depthCallback);
	}
	else if (type == VideoModule::eFrameType_RGB)
	{
		_colorCallback.registerFrameCallback(frameCallback);

		if (_colorStream.isValid())
			_colorStream.addNewFrameListener(&_colorCallback);
	}
	
}

