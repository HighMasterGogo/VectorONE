#pragma once

#include <QString>
#include "decoder.h"

struct blob
{
	float minX;
	float minY;
	float maxX;
	float maxY;
	float cX;
	float cY;
};

struct region
{
	uint8_t	id;
	int minX;
	int minY;
	int maxX;
	int maxY;
	int width;
	int height;
	int pixelIdx;
	int pixelCount;
	uint8_t maxLum;
};

struct blobDataHeader
{
	int64_t frameId;
	float avgMasterOffset;
	int blobCount;
	int regionCount;
	int foundRegionCount;
	int totalTime;
};

struct MaskElement
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

class LiveTracker
{
public:

	LiveTracker();
	~LiveTracker();

	int			id;
	uint32_t	serial;
	QString		name;
	int			version;

	int			exposure;
	int			iso;
	int			threshold;
	int			sensitivity;
	int			frameDuration;

	float		frames;
	float		data;
	float		fps;
	float		dataRecv;
	float		avgMasterOffset;
	int64_t		latestFrameId;

	bool		selected;
	bool		active;
	bool		connected;
	bool		loaded;
	int			interactMode;

	uint8_t		frameData[VID_W * VID_H * 3];
	MaskElement	maskVisualData[64 * 44];
	uint8_t		maskData[64 * 44];
	uint8_t		markerData[1024 * 10];
	int			markerDataSize;

	void updateStats();

	void changeMask(int X, int Y, bool Value);
	bool getMask(int X, int Y);
	void generateMask();
	void setMask(uint8_t* Data);

private:

};