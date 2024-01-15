#pragma once

#include "Processor.h"

class Invert : public Processor
{
public:
	Invert() : ProcessorInst(Invert) { }

	static Registrar<Invert> registrar;

	void ProcessImage(float fElapsedTime, frame& input, frame& output)
	{
		for (int i = 0; i < nFrameWidth; i++)
		{
			for (int j = 0; j < nFrameHeight; j++)
			{
				output.set(i, j, 1.0f - input.get(i, j));
			}
		}
	}
};

RegistrarInst(Invert);
