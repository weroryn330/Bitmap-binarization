#pragma once

//#include <Windows.h>
#include <ObjIdl.h>
//#include <gdiplus.h>
//#include <iostream>
//#include <fstream>
#include <vector>
//#include <assert.h>
//#include <stdio.h>
//#include <chrono>
#include <thread>
//#include <ctime>

#include <funkcjeEksportowane.h>
//#include "algorytm.h"

#pragma warning(disable:4996)
//#pragma comment (lib,"Gdiplus.Lib")

typedef int(_fastcall* Main)(unsigned char*, unsigned char*, int, int, int);
HINSTANCE dllHandle = LoadLibrary("DLL_asm.dll");
Main asmAlg = (Main)GetProcAddress(dllHandle, "Main");

void startAsm(unsigned char* buf, unsigned char* part, int start, int finish, int intensity)
{
	asmAlg(buf, part, start, finish, intensity);
}


class ReadBitmap
{
	//Gdiplus::Bitmap* startBitmap;
	clock_t time;
	double doubleTime;
public:
	ReadBitmap()
		//: readBitmap(nullptr)
	{}
	bool readBMP(std::string fPath, int intensity, int threads, int isAsm)
	{

		// liczenie czasu
		//clock_t timeStart = clock();

		FILE* input = fopen(fPath.c_str(), "rb");
		FILE* output;
		if (isAsm == 1) {
			output = fopen("asmOutput.bmp", "wb");
		}
		else {
			output = fopen("cppOutput.bmp", "wb");
		}


		/*** przepisanie naglowka z input do output ***/
		char header[54];											// tablica z informacjami z naglowka bitmapy
		fread(header, 1, 54, input);
		fwrite(header, 1, 54, output);

		int width = *(int*)(header + 18);							// szerokosc obrazu w pikselach
		int hight = *(int*)(header + 22);							// wysokosc obrazu w pikselach

		unsigned char* buf = new unsigned char[width * hight * 3];	// bufor wejsciowy z R, G, B


		/*** wczytanie bitmapy do bufora ***/
		fread(buf, sizeof(unsigned char), width * hight * 3, input);
		fclose(input);


		int linesPerThread = hight / threads;						// ile wierszy przypada na jeden watek
		std::vector<int> indexesPerThread;							// indeksy od-do dla kazdego z watkow


		/*** przyporzadkowanie czesci bitmapy (poprzez indeksy) dla kazdego z watkow ***/
		for (int i = 0; i < threads; i++) {
			if (i == 0) {
				indexesPerThread.push_back(0);
				indexesPerThread.push_back(linesPerThread * width * 3);
			}
			else {
				indexesPerThread.push_back(i * linesPerThread * width * 3);
				if (i == threads - 1) {
					indexesPerThread.push_back(width * hight * 3);
					break;
				}
				indexesPerThread.push_back((i + 1) * linesPerThread * width * 3);
			}
		}


		/*** alokacja pamieci na gotowa bitmape ***/
		std::vector<unsigned char*> parts;
		int x = 0;
		int y = 1;
		for (int i = 0; i < threads; i++) {
			int size = indexesPerThread[y] - indexesPerThread[x];
			unsigned char* temp = new unsigned char[size];
			parts.push_back(temp);
			x += 2;
			y += 2;
		}

		x = 0;
		y = 1;

		auto clockStart = std::chrono::steady_clock::now();
		// liczenie czasu
		//clock_t timeStart = clock();

		/*** watki programu ***/
		std::vector<std::thread> thread;
		for (int i = 0; i < threads; i++) {
			if (isAsm == 1) {
				thread.push_back(std::thread(&startAsm, buf, std::ref(parts[i]), indexesPerThread[x], indexesPerThread[y], intensity));
			}
			else if (isAsm == 0) {
				thread.push_back(std::thread(&cppAlg, buf, std::ref(parts[i]), indexesPerThread[x], indexesPerThread[y], intensity));
			}
			x += 2;
			y += 2;
		}
		for (int i = 0; i < threads; i++) {
			if (thread[i].joinable()) {
				thread[i].join();
			}
		}

		auto clockEnd = std::chrono::steady_clock::now();
		auto finalTime = clockEnd - clockStart;
		doubleTime = std::chrono::duration<double, std::milli>(finalTime).count();

		x = 0;
		y = 1;


		/*** zapis gotowych czesci bitmapy do output ***/
		for (int i = 0; i < parts.size(); i++) {
			int size = indexesPerThread[y] - indexesPerThread[x];
			fwrite(parts[i], sizeof(unsigned char), size, output);
			x += 2;
			y += 2;
		}
		fclose(output);


		/*** zwolnienie pamieci programu ***/
		for (int i = 0; i < threads; i++) {
			delete parts[i];
		}

		parts.clear();
		delete[] buf;
		//doubleTime = double(clock() - timeStart) / CLOCKS_PER_SEC;
		return true;
	}
	~ReadBitmap() {};

	std::string getTime()
	{
		return std::string(std::to_string(doubleTime));
	}
};