#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "funkcjeEksportowane.h"
#include <iostream>
#include "algorytm.h"

/*
* Algorytm odpowiada za utworzenie wyjsciowej, czarno-bialej bitmapy 
* na podstawie porownania wartosci piksela z ustalonym progiem.
*/
void cppAlg (unsigned char* buf, unsigned char* part, int start, int end, int intensity)
{
	int iZero = 0; // pierwszy iterator po skladowych piksela
	for (int i = start; i < end; i += 3) { // i - drugi iterator po skladowych piksela
		// porownanie wartosci piksela z oryginalnej bitmapy z progiem
		if (((buf[i] + buf[i + 1] + buf[i + 2]) / 3) > intensity) { 
			// ustawienie wartosci skladowych na 255 w przypadku jesli wartosc piksela jest wieksza od zadanego progu
			part[iZero] = 0xFF;
			part[iZero + 1] = 0xFF;
			part[iZero + 2] = 0xFF;
		}
		else { // w przeciwnym wypadku else
			// ustawienie wartosci skladowych na 0 w przeciwnym przypadku
			part[iZero] = 0x00;
			part[iZero + 1] = 0x00;
			part[iZero + 2] = 0x00;
		}
		iZero += 3; // przejscie na poczatek kolejnego piksela
	}
}
