#pragma once
extern "C" {
	#include "libavcodec/avcodec.h"
}

class VideoDecoder {
public:
	VideoDecoder(AVCodecContext* codec_context);
	VideoDecoder(AVCodecParameters* codec_param);
	~VideoDecoder(); void operator()(AVFrame* frame, int &finished, AVPacket* packet);
	unsigned width() const;
	unsigned height() const;
	AVPixelFormat pixel_format() const;
	AVRational time_base() const;
private:
	AVCodecContext* codec_context_{};
	bool decode(AVFrame* frame, AVPacket* packet);
};
