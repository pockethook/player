#include "video_decoder.h"
#include "ffmpeg.h"

VideoDecoder::VideoDecoder(AVCodecContext* codec_context) :
	codec_context_{codec_context} {
	avcodec_register_all();
	const auto codec_video =
		avcodec_find_decoder(codec_context_->codec_id);
	if (!codec_video) {
		throw ffmpeg::Error{"Unsupported video codec"};
	}
	ffmpeg::check(avcodec_open2(
		codec_context_, codec_video, nullptr));
}
VideoDecoder::VideoDecoder(AVCodecParameters* codec_param) {
	const auto codec_video =
		avcodec_find_decoder(codec_param->codec_id);
	if (!codec_video) {
		throw ffmpeg::Error{ "Unsupported video codec" };
	}
	codec_context_ = avcodec_alloc_context3(codec_video);
	avcodec_parameters_to_context(codec_context_, codec_param);
	ffmpeg::check(avcodec_open2(
		codec_context_, codec_video, nullptr));
}

VideoDecoder::~VideoDecoder() {
	avcodec_free_context(&codec_context_);
}

void VideoDecoder::operator()(
	AVFrame* frame, int &finished, AVPacket* packet) {
	//ffmpeg::check(avcodec_decode_video2(
	//	codec_context_, frame, &finished, packet));
	
	//maybe there need a check
	finished = decode(frame, packet);
}

unsigned VideoDecoder::width() const {
	return codec_context_->width;
}

unsigned VideoDecoder::height() const {
	return codec_context_->height;
}

AVPixelFormat VideoDecoder::pixel_format() const {
	return codec_context_->pix_fmt;
}

AVRational VideoDecoder::time_base() const {
	return codec_context_->time_base;
}
bool VideoDecoder::decode(AVFrame* frame, AVPacket* packet) {
	int send_packet = avcodec_send_packet(codec_context_, packet);
	int receive_frame = avcodec_receive_frame(codec_context_,frame);
	//finished = (send_packet == 0) && (receive_frame == 0);
	return (send_packet == 0) && (receive_frame == 0);
}
