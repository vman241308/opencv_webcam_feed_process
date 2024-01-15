/*
	8-Bits Of Image Processing You Should Know
	"Colin, at least you'll always get 700s now..." - javidx9

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Instructions:
	~~~~~~~~~~~~~
	Choose algorithm 1-8, instructions on screen


	Relevant Video: https://youtu.be/mRM5Js3VLCk

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, �OneLoneCoder 2019
*/

#include <vector>
#include <typeinfo>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "escapi.h"
#include "Processor.h"

// Add processors here
#include "Threshold.h"
#include "Adaptive.h"
#include "Motion.h"
#include "Lowpass.h"
#include "Convolution.h"
#include "Sobel.h"
#include "Morpho.h"
#include "Median.h"
#include "Invert.h"
#include "Normalize.h"
#include "Cutout.h"

struct ProcItem
{
	Processor* proc;
	int index;
};

class WebCam_ImageProcessing : public olc::PixelGameEngine
{
public:
	WebCam_ImageProcessing()
	{
		sAppName = "WebCam ImageProcessing";
	}

	union RGBint
	{
		int rgb;
		unsigned char c[4];
	};

	int nCameras = 0;
	int sCamera = 0;
	SimpleCapParams capture;

public:
	bool OnUserCreate() override
	{
		// Initialise webcam to screen dimensions
		nCameras = setupESCAPI();
		if (nCameras == 0)	return false;
		sCamera = nCameras - 1; // always pick the last camera in list
		capture.mWidth = nFrameWidth;
		capture.mHeight = nFrameHeight;
		capture.mTargetBuf = new int[nFrameWidth * nFrameHeight];
		if (initCapture(sCamera, &capture) == 0)	return false;

		Processor::SetEngine(this);

		// start with the Threshold processor
		const type_info& procType = typeid(Threshold);
		auto factory = Processor::registry().find(procType)->second.func;

		int index = 0;
		for (auto& info : Processor::registry())
		{
			if (info.first == procType) { break; }
			index++;
		}

		proclist.push_back(ProcItem{ factory(), index });

		return true;
	}

	void DrawFrame(frame& f, int x, int y)
	{
		for (int i = 0; i < nFrameWidth; i++)
			for (int j = 0; j < nFrameHeight; j++)
			{
				int c = (int)min(max(0.0f, f.pixels[j * nFrameWidth + i] * 255.0f), 255.0f);
				Draw(x + i, y + j, olc::Pixel(c, c, c));
			}
	}

	frame source, input, output;
	std::vector<ProcItem> proclist;

	bool OnUserUpdate(float fElapsedTime) override
	{
		doCapture(sCamera);
		while (isCaptureDone(sCamera) == 0) { }

		for (int y = 0; y < capture.mHeight; y++)
		{
			for (int x = 0; x < capture.mWidth; x++)
			{
				RGBint col;
				int id = y * capture.mWidth + x;
				col.rgb = capture.mTargetBuf[id];
				source.pixels[y * nFrameWidth + x] = (float)col.c[1] * 0.3f / 255.0f + (float)col.c[2] * 0.6f / 255.0f + (float)col.c[3] * 0.1f / 255.0f;
			}
		}

		int procCount = Processor::registry().size();
		proclist.back().proc->ProcessKeys(fElapsedTime);
		int procIndex = proclist.back().index;

		if (GetKey(olc::Key::PGDN).bReleased && procIndex > 0) procIndex--;
		if (GetKey(olc::Key::PGUP).bReleased && procIndex + 1 < procCount) procIndex++;
		bool newProc = GetKey(olc::Key::ENTER).bReleased;

		if (proclist.back().index != procIndex || newProc)
		{
			int jj = 0;
			for (auto& info : Processor::registry())
			{
				if (jj == procIndex)
				{
					// If not adding a new processor with the ENTER key
					if (newProc == false)
					{
						// clean up memory and pop one
						auto p = proclist.back();
						delete(p.proc);
						proclist.pop_back();
					}

					// Add the current proc
					proclist.push_back(ProcItem{ info.second.func(), procIndex });
					break;
				}

				jj++;
			}
		}

		if (GetKey(olc::Key::DEL).bReleased && proclist.size() > 1)
		{
			auto p = proclist.back();
			delete(p.proc);
			proclist.pop_back();
		}

		input = source;
		for (ProcItem proc : proclist)
		{
			proc.proc->ProcessImage(fElapsedTime, input, output);
			input = output;
		}

		// DRAW STUFF ONLY HERE
		Clear(olc::DARK_BLUE);
		DrawFrame(source, 10, 10);
		DrawFrame(proclist.size() == 0 ? source : output, 340, 10);

		DrawString(150, 255, "INPUT");
		DrawString(480, 255, "OUTPUT");

		DrawString(10, 275, "Choose Processor using PGUP or PGDN keys");
		DrawString(10, 285, "Add chosen processor using ENTER");
		DrawString(10, 295, "Remove last processor using DEL");

		int posx = 10;
		for (ProcItem pi : proclist)
		{
			std::string out = pi.proc->GetName();
			int w = out.length() * 10;
			DrawString(posx, 325, out);
			posx += w;
		}

		if (proclist.size() > 0)
		{
			proclist.back().proc->DrawUI(10, 355, 10);
		}

		if (GetKey(olc::Key::ESCAPE).bPressed) return false;
		return true;
	}
private:
	Processor* current;
};

int main()
{
	WebCam_ImageProcessing demo;
	if (demo.Construct(670, 460, 2, 2))
	{
		demo.Start();
		return 0;
	}
	else
	{
		return -1;
	}
}

