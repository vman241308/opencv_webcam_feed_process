#pragma once

#include "Processor.h"

class Sobel : public Processor
{
public:
	Sobel() : ProcessorInst(Sobel) { }

	static Registrar<Sobel> registrar;

	void ProcessImage(float fElapsedTime, frame& input, frame& output)
	{
		for (int i = 0; i < nFrameWidth; i++)
		{
			for (int j = 0; j < nFrameHeight; j++)
			{
				float fKernelSumH = 0.0f;
				float fKernelSumV = 0.0f;

				for (int n = -1; n < +2; n++)
				{
					for (int m = -1; m < +2; m++)
					{
						fKernelSumH += input.get(i + n, j + m) * kernel_sobel_h[(m + 1) * 3 + (n + 1)];
						fKernelSumV += input.get(i + n, j + m) * kernel_sobel_v[(m + 1) * 3 + (n + 1)];
					}
				}

				output.set(i, j, fabs((fKernelSumH + fKernelSumV) / 2.0f));
			}
		}
	}

private:
	float kernel_sobel_v[9] =
	{
		-1.0f, 0.0f, +1.0f,
		-2.0f, 0.0f, +2.0f,
		-1.0f, 0.0f, +1.0f,
	};
	float kernel_sobel_h[9] =
	{
		-1.0f, -2.0f, -1.0f,
		 0.0f, 0.0f, 0.0f,
		+1.0f, +2.0f, +1.0f,
	};
};

RegistrarInst(Sobel);
