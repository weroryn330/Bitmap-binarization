#pragma once

class Algorytm 
{
	int newPixel;
public:
	Algorytm() = default;
	Algorytm(int prog, int oldR, int oldG, int oldB)
	{
		if (((oldR + oldG + oldB) / 3) < prog)
		{
			newPixel = 0;
		}
		else
		{
			newPixel = 255;
		}
	};
	int startAlg(int prog, int oldR, int oldG, int oldB)
	{
		if (((oldR + oldG + oldB) / 3) < prog)
		{
			newPixel = 0;
		}
		else
		{
			newPixel = 255;
		}
	}
	int newPixelReturner() 
	{
		return newPixel;
	}
	~Algorytm() {};
};