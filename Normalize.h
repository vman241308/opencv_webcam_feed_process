#pragma once

#include "Processor.h"

class Normalize : public Processor
{
public:
	Normalize() : ProcessorInst(Normalize) { }

	~Normalize()
	{
	}

	static Registrar<Normalize> registrar;

	void ProcessKeys(float fElapsedTime)
	{
		// Respond to user input
		if (game->GetKey(olc::Key::DOWN).bHeld && (fNewLight - 0.1f) > fNewDark) fNewLight -= 0.1f * fElapsedTime;
		if (game->GetKey(olc::Key::UP).bHeld) fNewLight += 0.1f * fElapsedTime;
		if (game->GetKey(olc::Key::LEFT).bHeld) fNewDark -= 0.1f * fElapsedTime;
		if (game->GetKey(olc::Key::RIGHT).bHeld && (fNewDark + 0.1f) < fNewLight ) fNewDark += 0.1f * fElapsedTime;

		if (fNewLight <  0.1f) fNewLight = 0.1f;
		if (fNewLight >  5.0f) fNewLight = 5.0f;
		if (fNewDark  < -5.0f) fNewDark = -5.0f;
		if (fNewDark  >  4.9f) fNewDark =  4.9f;
	}

	void ProcessImage(float fElapsedTime, frame& input, frame& output)
	{
		float fDarkest = 1;
		float fLightest = 0;

		for (int i = 0; i < nFrameWidth; i++)
		{
			for (int j = 0; j < nFrameHeight; j++)
			{
				float t = input.get(i, j);
				if (t < fDarkest) { fDarkest = t; }
				if (t > fLightest) { fLightest = t; }
			}
		}

		// average over 10 frames to eliminate normalization flicker
		fPrevLight = fPrevLight * 0.9f + fLightest * 0.1;
		fPrevDark = fPrevDark * 0.9f + fDarkest * 0.1;

		for (int i = 0; i < nFrameWidth; i++)
		{
			for (int j = 0; j < nFrameHeight; j++)
			{
				float t = input.get(i, j);
				t = (t - fPrevDark) * ((fNewLight - fNewDark) / (fPrevLight - fPrevDark)) + fNewDark;
				output.set(i, j, t);
			}
		}
	}

	virtual void DrawUI(int x, int y, int stepy)
	{
		game->DrawString(x, y, "Change new dark level with arrow LEFT and RIGHT");
		game->DrawString(x, y + stepy, "Current value = " + std::to_string(fNewDark));
		y += stepy + stepy;
		game->DrawString(x, y, "Change new light level with arrow UP and DOWN");
		game->DrawString(x, y + stepy, "Current value = " + std::to_string(fNewLight));
	}

private:
	float fNewDark = 0;
	float fNewLight = 1;
	float fPrevDark = 0;
	float fPrevLight = 1;
};

RegistrarInst(Normalize);
