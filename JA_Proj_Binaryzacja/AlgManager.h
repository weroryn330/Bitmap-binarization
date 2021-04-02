#pragma once

#include <string>

#include "ReadBitmap.h"

class AlgManager
	:
	public ReadBitmap
{
	int nThreads;		// ilosc watkow
	bool isAsm;			// ktora biblioteka
	std::string fName;	// nazwa pliku
	int intensity;		// prog

public:
	AlgManager()
		: ReadBitmap(), nThreads(1), isAsm(false), fName(""), intensity(-1)
	{}
	// TODO: poprawnosc danych
	void validateData() {}
	void startApp()
	{
		readBMP(fName, intensity, nThreads, isAsm);
	}
	void setnThreads(int _nThreads) { nThreads = _nThreads; }
	void setIsAsm(bool _isAsm) { isAsm = _isAsm; }
	void setfName(std::string _fName) { fName = _fName; }
	void setFilter(int _filter) { intensity = _filter; }
	void resetData()
	{
		nThreads = 1;
		isAsm = false;
		fName = "";
		intensity = -1;
	}
	~AlgManager() {};
};