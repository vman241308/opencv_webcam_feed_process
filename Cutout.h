#pragma once

#include "Processor.h"

class Cutout : public Processor
{
public:
	Cutout() : ProcessorInst(Cutout) { }

	static Registrar<Cutout> registrar;

	void ProcessKeys(float fElapsedTime)
	{
		// Respond to keyboard keys
		if (game->GetKey(olc::Key::SHIFT).bHeld)
		{
			if (game->GetKey(olc::Key::LEFT).bHeld) fLeft += 0.1f * fElapsedTime;
			if (fLeft + 0.1f > fRight) fLeft = fRight - 0.1f;
			if (game->GetKey(olc::Key::RIGHT).bHeld) fRight -= 0.1f * fElapsedTime;
			if (fRight - 0.1f < fLeft) fRight = fLeft + 0.1f;
			if (game->GetKey(olc::Key::UP).bHeld) fTop += 0.1f * fElapsedTime;
			if (fTop + 0.1f > fBottom) fTop = fBottom - 0.1f;
			if (game->GetKey(olc::Key::DOWN).bHeld) fBottom -= 0.1f * fElapsedTime;
			if (fBottom - 0.1f < fTop) fBottom = fTop + 0.1f;
		}
		else
		{
			if (game->GetKey(olc::Key::LEFT).bHeld) fLeft -= 0.1f * fElapsedTime;
			if (fLeft < 0.0f) fLeft = 0.0f;
			if (game->GetKey(olc::Key::RIGHT).bHeld) fRight += 0.1f * fElapsedTime;
			if (fRight > 1.0f) fRight = 1.0f;
			if (game->GetKey(olc::Key::UP).bHeld) fTop -= 0.1f * fElapsedTime;
			if (fTop < 0.0f) fTop = 0.0f;
			if (game->GetKey(olc::Key::DOWN).bHeld) fBottom += 0.1f * fElapsedTime;
			if (fBottom > 1.0f) fBottom = 1.0f;
		}

	}

	void ProcessImage(float fElapsedTime, frame& input, frame& output)
	{
		for (int i = 0; i < nFrameWidth; i++)
		{
			for (int j = 0; j < nFrameHeight; j++)
			{
				float fi = float(i) / float(nFrameWidth);
				float fj = float(j) / float(nFrameHeight);
				bool inside = fi > fLeft && fi < fRight&& fj > fTop && fj < fBottom;
				output.set(i, j, inside ? input.get(i,j) : 0.0f);
			}
		}
	}

	void DrawUI(int x, int y, int stepy)
	{
		game->DrawString(x, y, "Change Cutout mode with arrow LEFT and RIGHT");
		game->DrawString(x, y + stepy, "Area = T:" + std::to_string(int(fTop * nFrameHeight)) +
											", B:" + std::to_string(int(fBottom * nFrameHeight)) +
											", L:" + std::to_string(int(fLeft * nFrameHeight)) +
											", R:" + std::to_string(int(fRight * nFrameHeight)) +
			"");
	}

private:
	float fTop = 0.25f, fLeft = 0.25f, fRight= 0.75, fBottom = 0.75;
};

RegistrarInst(Cutout);

