
#define __STDC_CONSTANT_MACROS
#define _TIMECHECK

extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
}

#include <sys/time.h>
#include "MPEGCoder.h"

namespace MPEGLib
{


	struct sRGB
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	void RGBToYUV420(uint8_t* Y, uint8_t* U, uint8_t* V, int linesize1, int linesize2, int linesize3, const uint8_t* RGB, int frameWidth, int frameHeight)
	{
		sRGB* rgb = (sRGB*)RGB;

		for (int i = 0; i < frameHeight / 2; i++)
		{
			for (int j = 0; j < frameWidth / 2; j++)
			{

				uint16_t r, g, b;
				int rgbIndex = 2 * i*frameWidth + 2 * j;
				r = rgb[rgbIndex].r;
				g = rgb[rgbIndex].g;
				b = rgb[rgbIndex].b;

				int yIndex = 2 * (i*linesize1 + j); 
				Y[yIndex] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
				U[i*linesize2 + j] = ((-38 * r - 74 * g + 112 * b) >> 8) + 128;
				V[i*linesize3 + j] = ((112 * r - 94 * g - 18 * b) >> 8) + 128;

				rgbIndex++; // = 2 * i*frameWidth + 2 * j + 1;
				r = rgb[rgbIndex].r;
				g = rgb[rgbIndex].g;
				b = rgb[rgbIndex].b;

				yIndex++;
				Y[yIndex] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;

				rgbIndex += (frameWidth - 1); //= (2 * i + 1)*frameWidth + 2 * j;
				r = rgb[rgbIndex].r;
				g = rgb[rgbIndex].g;
				b = rgb[rgbIndex].b;

				yIndex += (linesize1 - 1);
				Y[yIndex] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
				rgbIndex++; // = (2 * i + 1)*frameWidth + 2 * j + 1;
				r = rgb[rgbIndex].r;
				g = rgb[rgbIndex].g;
				b = rgb[rgbIndex].b;
				yIndex++;
				Y[yIndex] = ((66 * r + 129 * g + 25 * b) >> 8) + 16;
			}
		}

 
	}

	void YUV420ToRGB(uint8_t* RGB, const uint8_t* Y, const uint8_t* U, const uint8_t* V, int frameWidth, int frameHeight, int linesize1, int linesize2, int linesize3)
	{

		sRGB* rgb = (sRGB*)RGB;

		for (int i = 0; i < frameHeight / 2; i++)
		{
			for (int j = 0; j < frameWidth / 2; j++)
			{
				float r, g, b;
		
				
				int16_t yVal = Y[2 * i*linesize1 + 2 * j];
				int16_t vVal = V[i*linesize3 + j] - 128;
				int16_t uVal = U[i*linesize2 + j] - 128;
				int rgbIndex = 2 * i*frameWidth + 2 * j;

				float r_UVValue = 1.596*vVal;
				float g_UVValue = -0.813*vVal - 0.391*uVal;
				float b_UVValue = 2.018*uVal;

				float y_value = 1.164*(yVal - 16);


				r = y_value + r_UVValue;
				g = y_value + g_UVValue;
				b = y_value + b_UVValue;

				if (r < 0) r = 0;
				if (g < 0) g = 0;
				if (b < 0) b = 0;
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;


				rgb[rgbIndex].r = r;
				rgb[rgbIndex].g = g;
				rgb[rgbIndex].b = b;

				yVal = Y[2 * i*linesize1 + 2 * j + 1];
				rgbIndex = 2 * i*frameWidth + 2 * j + 1;

				y_value = 1.164*(yVal - 16);


				r = y_value + r_UVValue;
				g = y_value + g_UVValue;
				b = y_value + b_UVValue;

				if (r < 0) r = 0;
				if (g < 0) g = 0;
				if (b < 0) b = 0;
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;


				rgb[rgbIndex].r = r;
				rgb[rgbIndex].g = g;
				rgb[rgbIndex].b = b;


				yVal = Y[(2 * i + 1)*linesize1 + 2 * j];
				rgbIndex = (2 * i + 1)*frameWidth + 2 * j;

				y_value = 1.164*(yVal - 16);


				r = y_value + r_UVValue;
				g = y_value + g_UVValue;
				b = y_value + b_UVValue;


				if (r < 0) r = 0;
				if (g < 0) g = 0;
				if (b < 0) b = 0;
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;


				rgb[rgbIndex].r = r;
				rgb[rgbIndex].g = g;
				rgb[rgbIndex].b = b;

				yVal = Y[(2 * i + 1)*linesize1 + 2 * j + 1];
				rgbIndex = (2 * i + 1)*frameWidth + 2 * j + 1;

				y_value = 1.164*(yVal - 16);


				r = y_value + r_UVValue;
				g = y_value + g_UVValue;
				b = y_value + b_UVValue;


				if (r < 0) r = 0;
				if (g < 0) g = 0;
				if (b < 0) b = 0;
				if (r > 255) r = 255;
				if (g > 255) g = 255;
				if (b > 255) b = 255;


				rgb[rgbIndex].r = r;
			rgb[rgbIndex].g = g;
				rgb[rgbIndex].b = b;
 
			}
		}
	}


	void InitailizeLibrary()
	{
		avcodec_register_all();
	}


	MPEGEncoder::MPEGEncoder() : m_frame(NULL), m_codecContext(NULL)
	{
	}


	MPEGEncoder::~MPEGEncoder()
	{
		
	}

	MPEGDecoder::MPEGDecoder() : m_frame(NULL), m_codecContext(NULL), m_avpkt(NULL)
	{

	}

	MPEGDecoder::~MPEGDecoder()
	{
		
		
	}

	void MPEGEncoder::end()
	{
		if (m_codecContext)
		{
			avcodec_close(m_codecContext);
			av_free(m_codecContext);
		}

		if (m_frame)
		{
			av_freep(&m_frame->data[0]);
			av_frame_free(&m_frame);
		}
	}


	int MPEGEncoder::initialize(int codecId, int frameWidth, int frameHeight)
	{

		m_codec = avcodec_find_encoder(AV_CODEC_ID_MPEG1VIDEO);
		if (!m_codec) {
			fprintf(stderr, "Codec not found\n");
			return -1;
		}

		m_codecContext = avcodec_alloc_context3(m_codec);
		if (!m_codecContext) {
			fprintf(stderr, "Could not allocate video codec context\n");
			return -2;
		}


		/* put sample parameters */
		m_codecContext->bit_rate = 400000;
		/* resolution must be a multiple of two */
		m_codecContext->width = frameWidth;
		m_codecContext->height = frameHeight;
		/* frames per second */
		m_codecContext->time_base.num = 1;
		m_codecContext->time_base.den = 25;
		/* emit one intra frame every ten frames
		* check frame pict_type before passing frame
		* to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
		* then gop_size is ignored and the output of encoder
		* will always be I frame irrespective to gop_size
		*/
		m_codecContext->gop_size = 10;
		m_codecContext->max_b_frames = 1;
		m_codecContext->pix_fmt = AV_PIX_FMT_YUV420P;


		/* open it */
		if (avcodec_open2(m_codecContext, m_codec, NULL) < 0) {
			fprintf(stderr, "Could not open codec\n");
			return -3;
		}

		m_frame = av_frame_alloc();
		if (!m_frame) {
			fprintf(stderr, "Could not allocate video frame\n");
			return -4;
		}
		m_frame->format = m_codecContext->pix_fmt;
		m_frame->width = m_codecContext->width;
		m_frame->height = m_codecContext->height;

		/* the image can be allocated by any means and av_image_alloc() is
		* ju#include <sys/time.h>st the most convenient way if av_malloc() is to be used */
		int ret = av_image_alloc(m_frame->data, m_frame->linesize, m_codecContext->width, m_codecContext->height,
			m_codecContext->pix_fmt, 32);
		if (ret < 0) {
			fprintf(stderr, "Could not allocate raw picture buffer\n");
			return -5;
		}

		m_framePts = 0;
		return 0;
	}

	int MPEGEncoder::EncodeImage(uint8_t* encodedBuffer, const uint8_t* RawRGBBuffer)
	{
		AVPacket pkt;
		int res = 0;

		av_init_packet(&pkt);
		pkt.data = NULL;    // packet data will be allocated by the encoder
		pkt.size = 0;

#ifdef _TIMECHECK
		struct timeval start, end;
		gettimeofday(&start, NULL);
#endif

		RGBToYUV420(m_frame->data[0], m_frame->data[1], m_frame->data[2], m_frame->linesize[0], m_frame->linesize[1], m_frame->linesize[2], RawRGBBuffer, m_frame->width, m_frame->height);

#ifdef _TIMECHECK
		gettimeofday(&end, NULL);

		long elapsed_sec = end.tv_sec - start.tv_sec;
		long elapsed_usec = end.tv_usec - start.tv_usec;
		if (elapsed_usec < 0)
		{
			elapsed_sec -= 1;
			elapsed_usec += 1000000;
		}
		
		printf("elapsed time : %d usec\n", elapsed_usec);

#endif
		m_framePts++;
		int got_output = 1;


		int ret = avcodec_encode_video2(m_codecContext, &pkt, m_frame, &got_output);
		if (ret < 0) {
			fprintf(stderr, "Error encoding frame\n");
			return -1;
		}

		if (got_output == 1)
		{
			memcpy(encodedBuffer, pkt.data, pkt.size);
			res =  pkt.size;
		}

		av_free_packet(&pkt);

		return res;

	}

	int MPEGDecoder::initialize(int codecId)
	{
		m_codec = avcodec_find_decoder((AVCodecID)codecId);
		if (!m_codec) {
			fprintf(stderr, "Codec not found\n");
			return -1;
		}

		m_codecContext = avcodec_alloc_context3(m_codec);
		if (!m_codecContext) {
			fprintf(stderr, "Could not allocate video codec context\n");
			return -2;
		}

		if (m_codec->capabilities&CODEC_CAP_TRUNCATED)
			m_codecContext->flags |= CODEC_FLAG_TRUNCATED; /* we do not send complete frames */

		/* For some codecs, such as msmpeg4 and mpeg4, width and height
		MUST be initialized there because this information is not
		available in the bitstream. */

		/* open it */
		if (avcodec_open2(m_codecContext, m_codec, NULL) < 0) {
			fprintf(stderr, "Could not open codec\n");
			return -3;
		}

		m_frame = av_frame_alloc();
		if (!m_frame) {
			fprintf(stderr, "Could not allocate video frame\n");
			return -4;
		}


		m_avpkt = new AVPacket();
		return 0;
	}

	void MPEGDecoder::end()
	{
		if (m_avpkt)
			delete m_avpkt;
		if (m_codecContext)
		{
			avcodec_close(m_codecContext);
			av_free(m_codecContext);
		}

		if (m_frame)
			av_frame_free(&m_frame);
	}

	int MPEGDecoder::DecodeImage(uint8_t* decodedBuffer, const uint8_t* encodedImageBuffer, int encodedSize)
	{
	
	
		m_avpkt->size = encodedSize;
		m_avpkt->data = (uint8_t*)encodedImageBuffer;

		int got_frame = 1;
		int len = 0;
		while (m_avpkt->size > 0)
		{

			len = avcodec_decode_video2(m_codecContext, m_frame, &got_frame, m_avpkt);

			if (got_frame)
			{
				YUV420ToRGB(decodedBuffer, m_frame->data[0], m_frame->data[1], m_frame->data[2], m_frame->width, m_frame->height, m_frame->linesize[0], m_frame->linesize[1], m_frame->linesize[2]);
			
			}

			if (m_avpkt->data) {
				m_avpkt->size -= len;
				m_avpkt->data += len;
			}

		}

		

		return len;
	}


}
