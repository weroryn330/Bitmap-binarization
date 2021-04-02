#include <iostream>>
#include <thread>
#include <vector>
#include <Windows.h>

// wektor
std::vector<std::thread> vecOfThreads;

typedef int(_fastcall* MyProc1)(int, int, int);
typedef int(_fastcall* MyProc2)(int, int, int);
typedef int(_fastcall* MyProc3)(int, int, int);

void func(int tid) {
    std::cout << "Launched by thread " << tid << std::endl;
}

void funVectWatkow(bool biblioteka, int liczbaWatkow, HINSTANCE dllHandle)
{
    std::vector<std::thread> vecOfThreads1;
    std::vector<std::thread> vecOfThreads2;
    std::vector<std::thread> vecOfThreads3;
    if (biblioteka)
    {
        // C++
    }
    else
    {
        MyProc1 function1 = (MyProc1)GetProcAddress(dllHandle, "MyProc1");
        MyProc2 function2 = (MyProc2)GetProcAddress(dllHandle, "MyProc2");
        MyProc3 function3 = (MyProc3)GetProcAddress(dllHandle, "MyProc3");

        function1(2, 4, 6);

        for (int i = 0; i < liczbaWatkow; i++)
        {
            vecOfThreads1.push_back(std::thread(function1, 1, 2, 3));
            vecOfThreads2.push_back(std::thread(function2, 3, 4, 5));
            vecOfThreads3.push_back(std::thread(function3, 5, 6, 7));
        }
        for (int i = 0; i < liczbaWatkow; i++)
        {
            vecOfThreads1[i].join();
            vecOfThreads2[i].join();
            vecOfThreads3[i].join();
        }
    }
}
