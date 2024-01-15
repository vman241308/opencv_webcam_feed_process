#pragma once

#include "olcPixelGameEngine.h"

int nFrameWidth = 320;
int nFrameHeight = 240;

struct frame
{
	float* pixels = nullptr;

	frame()
	{
		pixels = new float[nFrameWidth * nFrameHeight];
	}

	~frame()
	{
		delete[] pixels;
	}

	float get(int x, int y)
	{
		if (x >= 0 && x < nFrameWidth && y >= 0 && y < nFrameHeight)
		{
			return pixels[y * nFrameWidth + x];
		}
		else
			return 0.0f;
	}

	void set(int x, int y, float p)
	{
		if (x >= 0 && x < nFrameWidth && y >= 0 && y < nFrameHeight)
		{
			pixels[y * nFrameWidth + x] = p;
		}
	}


	void operator=(const frame& f)
	{
		memcpy(this->pixels, f.pixels, nFrameWidth * nFrameHeight * sizeof(float));
	}
};

