#ifndef _MPEGCODER_H_
#define _MPEGCODER_H_

class AVCodec;
class AVCodecContext;
class AVFrame;
class AVPacket;

namespace MPEGLib
{

	class MPEGEncoder
	{
	public:
		MPEGEncoder();
		~MPEGEncoder();


		int initialize(int codecId, int frameWidth, int frameHeight);
		int EncodeImage(uint8_t* encodedBuffer, const uint8_t* RawRGBBuffer);
		void end();
	

	private:
		AVCodec*			m_codec;
		AVCodecContext*		m_codecContext;
		AVFrame*			m_frame;
		int					m_framePts;

	};
		
	class MPEGDecoder
	{
	public:
		MPEGDecoder();
		~MPEGDecoder();
		
		int initialize(int codecId);
		int DecodeImage(uint8_t* decodedBuffer, const uint8_t* encodedImageBuffer, int encodedSize);
		void end();

	private:

		AVCodec*			m_codec;
		AVCodecContext*		m_codecContext;
		AVFrame*			m_frame;
		AVPacket*			m_avpkt;


	};

	void InitailizeLibrary();

}

#endif
