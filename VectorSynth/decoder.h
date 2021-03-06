#pragma once

#include <Vector>

#include <QMutex>
#include <QVector2D>
#include <QMatrix4x4>

#include <opencv2\core.hpp>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\video.hpp>

extern "C"
{
	#include <libavcodec/avcodec.h>
	#include <libavdevice/avdevice.h>
	#include <libavfilter/avfilter.h>
	#include <libavformat/avformat.h>
	#include <libavformat/avio.h>
	#include <libavutil/avutil.h>
	#include <libpostproc/postprocess.h>
	#include <libswresample/swresample.h>
	#include <libswscale/swscale.h>
}

#define VID_W		1024
#define VID_H		704
#define INBUF_SIZE	8192

class Marker2D
{
public:

	int				markerId;
	int				trackerId;
	QVector2D		pos;	
	QVector2D		distPos;
	QVector4D		bounds;
	QVector3D		worldRayD;
};

class Decoder
{
public:
	Decoder();
	~Decoder();

	int				frameSkip;
	bool			drawUndistorted;
	volatile bool	findCalibrationSheet;
	bool			blankFrame;
	int				dataRecvBytes;
	int				newFrames;
	uint8_t			frameMaskData[64 * 44];

	QMutex									calibMutex;
	int										calibImageCount;
	std::vector<std::vector<cv::Point2f>>	calibImagePoints;

	bool DoDecode(uint8_t* Data, int Len);
	bool DoDecodeSingleFrame(uint8_t* Data, int Len, int* Consumed);
	bool isCalibrating() { return _calibrating;	};
	uint8_t* GetFrameMatData() { return colMat.data; }
	void TransferFrameToBuffer(uint8_t* Data);
	void ShowBlankFrame();
	void ProcessFrameLive();
	void ProcessFrame();
	QList<Marker2D> ProcessFrameNewMarkers();

private:

	// LibAV
	const AVCodec*			codec;
	AVCodecParserContext*	parser;
	AVCodecContext*			c = NULL;
	AVFrame*				frame;
	AVFrame*				frameRGB;
	uint8_t					inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
	AVPacket*				pkt;
	SwsContext*				sws;

	// OpenCV
	cv::Ptr<cv::SimpleBlobDetector> blobDetector;
	cv::Mat cvFrame;
	cv::Mat colMat;
	cv::Mat undistortMat;
	cv::Mat maskMat;

	// Calibration
	bool		_calibrating;
	
	// Frame parsing.
	int			destWidth;
	int			destHeight;
	int			_parsedFrames;
	int			_frameLimit;
	uint8_t		_postFrameData[VID_W * VID_H];

	bool _Decode();
	void _initOpenCV();
	void _findCalibrationSheet();
	void _CreateFrameGray();
};