#pragma once

#ifdef CPPLIBRARY_EXPORTS
#define CPPLIBRARY __declspec(dllexport)
#else
#define CPPLIBRARY __declspec(dllimport)
#endif

extern "C" CPPLIBRARY void cppAlg(unsigned char* buf, unsigned char* part, int start, int end, int intensity); 