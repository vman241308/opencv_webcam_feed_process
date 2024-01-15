#pragma once

#include "Processor.h"

class Convolution : public Processor
{
public:
	Convolution() : ProcessorInst(Convolution) { }

	static Registrar<Convolution> registrar;

	void ProcessKeys(float fElapsedTime)
	{
		// Respond to user input
		if (game->GetKey(olc::Key::LEFT).bHeld) pConvoKernel = kernel_blur;
		if (game->GetKey(olc::Key::RIGHT).bHeld) pConvoKernel = kernel_sharpen;
	}

	void ProcessImage(float fElapsedTime, frame& input, frame& output)
	{
		for (int i = 0; i < nFrameWidth; i++)
		{
			for (int j = 0; j < nFrameHeight; j++)
			{
				float fSum = 0.0f;
				for (int n = -1; n < +2; n++)
					for (int m = -1; m < +2; m++)
						fSum += input.get(i + n, j + m) * pConvoKernel[(m + 1) * 3 + (n + 1)];

				output.set(i, j, fSum);
			}
		}
	}

	void DrawUI(int x, int y, int stepy)
	{
		game->DrawString(x, y, "Change convolution mode with arrow LEFT and RIGHT");
		game->DrawString(x, y + stepy, "Current kernel = " + std::string((pConvoKernel == kernel_blur) ? "Blur" : "Sharpen"));
	}

private:
	float* pConvoKernel = kernel_blur;
	float kernel_blur[9] =
	{
		0.0f,   0.125,  0.0f,
		0.125f, 0.5f,   0.125f,
		0.0f,   0.125f, 0.0f,
	};
	float kernel_sharpen[9] =
	{
		0.0f,  -1.0f,  0.0f,
		-1.0f,  5.0f, -1.0f,
		0.0f,  -1.0f,  0.0f,
	};

};

RegistrarInst(Convolution);

