#pragma once

#include "Processor.h"
#include <Vector>

class Median : public Processor
{
public:
	Median() : ProcessorInst(Median) { }

	static Registrar<Median> registrar;

	void ProcessImage(float fElapsedTime, frame& input, frame& output)
	{
		std::vector<float> v;

		for (int i = 0; i < nFrameWidth; i++)
		{
			for (int j = 0; j < nFrameHeight; j++)
			{
				v.clear();

				for (int n = -2; n < +3; n++)
				{
					for (int m = -2; m < +3; m++)
					{
						v.push_back(input.get(i + n, j + m));
					}
				}

				std::vector<float>::iterator first = v.begin();
				std::vector<float>::iterator last = v.end();
				std::vector<float>::iterator middle = first + (last - first) / 2;
				std::nth_element(first, middle, last); // can specify comparator as optional 4th arg
				output.set(i, j, *middle);
			}
		}
	}
};

RegistrarInst(Median);
