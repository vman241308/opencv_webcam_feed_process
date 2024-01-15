#pragma once

#include "Processor.h"

enum MORPHOP { DILATION, EROSION, EDGE };

class Morpho : public Processor
{
public:
	Morpho() : ProcessorInst(Morpho) { }
	~Morpho()
	{
		activity.~frame();
	}

	static Registrar<Morpho> registrar;
	void ProcessKeys(float fElapsedTime)
	{
		// Respond to user input, order of testing is important
		if (game->GetKey(olc::Key::LEFT).bReleased && morph == EROSION) morph = EDGE;
		if (game->GetKey(olc::Key::LEFT).bReleased && morph == DILATION) morph = EROSION;
		if (game->GetKey(olc::Key::RIGHT).bReleased && morph == EROSION) morph = DILATION;
		if (game->GetKey(olc::Key::RIGHT).bReleased && morph == EDGE) morph = EROSION;

		if (game->GetKey(olc::Key::UP).bReleased) nMorphCount++;
		if (game->GetKey(olc::Key::DOWN).bReleased) nMorphCount--;

		if (nMorphCount > 10.0f) nMorphCount = 10.0f;
		if (nMorphCount < 1.0f) nMorphCount = 1.0f;
	}

	void ProcessImage(float fElapsedTime, frame& input, frame& output)
	{
		activity = input;

		switch (morph)
		{
		case DILATION:
			for (int n = 0; n < nMorphCount; n++)
			{
				if (n > 0) { activity = output; }
				output = activity;

				for (int i = 0; i < nFrameWidth; i++)
				{
					for (int j = 0; j < nFrameHeight; j++)
					{
						if (activity.get(i, j) > 0.5f)
						{
							output.set(i, j, 1.0f);
							output.set(i - 1, j, 1.0f);
							output.set(i + 1, j, 1.0f);
							output.set(i, j - 1, 1.0f);
							output.set(i, j + 1, 1.0f);
							output.set(i - 1, j - 1, 1.0f);
							output.set(i + 1, j + 1, 1.0f);
							output.set(i + 1, j - 1, 1.0f);
							output.set(i - 1, j + 1, 1.0f);
						}
						else
						{
							output.set(i, j, 0.0f);
						}
					}
				}
			}
			break;

		case EROSION:
			for (int n = 0; n < nMorphCount; n++)
			{
				if (n > 0) { activity = output; }

				output = activity;

				for (int i = 0; i < nFrameWidth; i++)
				{
					for (int j = 0; j < nFrameHeight; j++)
					{
						float sum =
							activity.get(i - 1, j - 1) +
							activity.get(i - 1, j + 0) +
							activity.get(i - 1, j + 1) +
							activity.get(i + 0, j - 1) +
							activity.get(i + 0, j + 1) +
							activity.get(i + 1, j - 1) +
							activity.get(i + 1, j + 0) +
							activity.get(i + 1, j + 1) +
							0;

						if (activity.get(i, j) > 0.5f && sum > 4.0f)
						{
							output.set(i, j, 1.0f);
						}
						else
						{
							output.set(i, j, 0.0f);
						}
					}
				}
			}
			break;

		case EDGE:
			output = activity;

			for (int i = 0; i < nFrameWidth; i++)
			{
				for (int j = 0; j < nFrameHeight; j++)
				{
					float sum =
						(activity.get(i - 1, j - 1) > 0.5 ? 1.0f : 0.0f) +
						(activity.get(i - 1, j + 0) > 0.5 ? 1.0f : 0.0f) +
						(activity.get(i - 1, j + 1) > 0.5 ? 1.0f : 0.0f) +
						(activity.get(i + 0, j - 1) > 0.5 ? 1.0f : 0.0f) +
						(activity.get(i + 0, j + 1) > 0.5 ? 1.0f : 0.0f) +
						(activity.get(i + 1, j - 1) > 0.5 ? 1.0f : 0.0f) +
						(activity.get(i + 1, j + 0) > 0.5 ? 1.0f : 0.0f) +
						(activity.get(i + 1, j + 1) > 0.5 ? 1.0f : 0.0f) +
						0.0f;

					if (activity.get(i, j) > 0.5f && sum == 8.0f)
					{
						output.set(i, j, 0.0f);
					}
					else
					{
						if (activity.get(i, j) > 0.5f)
						{
							output.set(i, j, 1.0f);
						}
						else
						{
							output.set(i, j, 0.0f);
						}
					}
				}
			}

			break;

		}
	}

	void DrawUI(int x, int y, int stepy)
	{
		game->DrawString(x, y, "Change morphing mode with arrow LEFT and RIGHT");
		y += stepy;
		if (morph == DILATION) game->DrawString(x, y, "Current operation = DILATION");
		if (morph == EROSION) game->DrawString(x, y, "Current operation = EROSION");
		if (morph == EDGE) game->DrawString(x, y, "Current operation = EDGE");

		if (morph != EDGE)
		{
			y += stepy;
			game->DrawString(x, y, "Change iterations with arrow UP and DOWN");
			y += stepy;
			game->DrawString(x, y, "Current iteration count = " + std::to_string(nMorphCount));
		}
	}

private:
	MORPHOP morph = DILATION;
	int nMorphCount = 1;
	frame activity;
};

RegistrarInst(Morpho);
