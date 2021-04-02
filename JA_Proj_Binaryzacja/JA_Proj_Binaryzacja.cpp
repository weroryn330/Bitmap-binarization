#include <Windows.h>
#include "AlgManager.h"

#include <CommCtrl.h>
#include <strsafe.h>
#pragma comment (lib,"Comctl32.Lib")

#define UD_MIN_POS 1    // dolna granica progu
#define UD_MAX_POS 64   // gorna granica progu

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//ULONG_PTR gdiplusToken;
AlgManager algManager;

HWND hlbl;
HWND hTrack;
UINT code;
HWND hUpDown, hEdit, hStatic;
LPNMUPDOWN lpnmud;
PAINTSTRUCT ps;

void createControls(HWND hWnd);
void createUDControls(HWND hWnd);
void updateLabel();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    const char* CLASS_NAME = "Sample Window Class";
	
    WNDCLASS wc = { };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    /* Tworzenie okna */
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "Projekt JA - Ustawienia",    
                                WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,            
                                CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
                                NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    std::string fPath;
    std::string score;

    switch (message)
    {
        case WM_CREATE:
        {
            createControls(hWnd);
            createUDControls(hWnd);

            /* Przyciski w oknie */
            CreateWindowW(L"BUTTON", L"Dodaj plik",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                150, 50, 200, 100, hWnd, (HMENU)1, NULL, NULL);
            CreateWindowW(L"BUTTON", L"Start!",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                300, 450, 200, 100, hWnd, (HMENU)2, NULL, NULL);
            CreateWindowW(L"BUTTON", L"ASM",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                150, 200, 200, 100, hWnd, (HMENU)3, NULL, NULL);
            CreateWindowW(L"BUTTON", L"C++",
                WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON,
                480, 200, 200, 100, hWnd, (HMENU)4, NULL, NULL);
            break;
        }
        case WM_NOTIFY: 
        {
            code = ((LPNMHDR)lParam)->code;
            if (code == UDN_DELTAPOS) {

                lpnmud = (NMUPDOWN*)lParam;

                int value = lpnmud->iPos + lpnmud->iDelta;

                if (value < UD_MIN_POS) {
                    value = UD_MIN_POS;
                }

                if (value > UD_MAX_POS) {
                    value = UD_MAX_POS;
                }

                algManager.setnThreads(value);

                const int asize = 4;
                wchar_t buf[asize];
                size_t cbDest = asize * sizeof(wchar_t);
                StringCbPrintfW(buf, cbDest, L"%d", value);

                SetWindowTextW(hStatic, buf);
            }
            break;
        }
        case WM_COMMAND:
        {
            switch LOWORD(wParam) {
            case 1:
            {
                OPENFILENAME ofn = { 0 };
                TCHAR szFile[260] = { 0 };
                ofn.lStructSize = sizeof(ofn);
                ofn.hInstance = GetModuleHandle(NULL);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = NULL;
                ofn.nFilterIndex = 1;
                ofn.lpstrFileTitle = NULL;
                ofn.nMaxFileTitle = 0;
                ofn.lpstrInitialDir = NULL;
                ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                if (GetOpenFileName(&ofn) != true)
                    break;

                fPath = szFile;
                algManager.setfName(fPath);
	            if (fPath[fPath.size() - 3] != 'b' ||
	                fPath[fPath.size() - 2] != 'm' ||
	                fPath[fPath.size() - 1] != 'p') // sprawdzanie czy bitmapa
                {
	                MessageBox(hWnd, "Niewłaściwy format pliku", "BŁĄD!", MB_ICONERROR );
	                break;
	            }
	            bool endFound = false;
	            std::vector<char> temp;
	            for (int i = 0; i < fPath.size(); i++) {
	                if (fPath[i] == '\\') {
	                    for (int j = i + 1; j < fPath.size(); j++) {
	                        if (fPath[j] == '\\') {
	                            endFound = false;
	                            break;
	                        }
	                        if (j == fPath.size() - 1) {
	                            endFound = true;
	                            i++;
	                        }
	                    }
	                }
	                if (endFound)
	                    temp.emplace_back(fPath[i]);
	            }
                std::string printfName = std::string(temp.begin(), temp.end());
	            TextOut(GetDC(hWnd), 450, 100, printfName.c_str(), printfName.size());
                break;
            }
            case 2:
                MessageBox(hWnd, "Start!", "Start",
                    MB_ICONINFORMATION | MB_OKCANCEL);
                algManager.startApp();
                score = "Bitmape przetworzono w : " + algManager.getTime()+ "ms.";
            	MessageBox(hWnd, score.c_str(), "Koniec",
                    MB_ICONINFORMATION | MB_OKCANCEL);
                algManager.setIsAsm(false);
            	break;
            case 3:
                algManager.setIsAsm(true);
                break;
            case 4:
                algManager.setIsAsm(false);
                break;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                TextOut(hdc, 150, 335, "Wybierz prog: ", 13);
                TextOut(hdc, 150, 400, "Wybierz liczbe watkow: ", 22);
                //TextOut(hdc, 480, 400, "Czas: ", 22);
                EndPaint(hWnd, &ps);
                break;
            }
        case WM_HSCROLL:
            updateLabel();
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void createControls(HWND hWnd)
{
    hlbl = CreateWindowW(L"Static", L"0", WS_CHILD | WS_VISIBLE,
        640, 335, 30, 20, hWnd, (HMENU)7, NULL, NULL);

    INITCOMMONCONTROLSEX icex;

    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

    hTrack = CreateWindowW(TRACKBAR_CLASSW, L"Trackbar Control",
        WS_CHILD | WS_VISIBLE | TBS_AUTOTICKS,
        330, 330, 300, 30, hWnd, (HMENU)8, NULL, NULL);

    SendMessageW(hTrack, TBM_SETRANGE, TRUE, MAKELONG(0, 255));
    SendMessageW(hTrack, TBM_SETPAGESIZE, 0, 15);
    SendMessageW(hTrack, TBM_SETTICFREQ, 17, 0);
    SendMessageW(hTrack, TBM_SETPOS, FALSE, 0);
}

void createUDControls(HWND hWnd)
{
    INITCOMMONCONTROLSEX ic;
    ic.dwSize = sizeof(INITCOMMONCONTROLSEX);
    ic.dwICC = ICC_UPDOWN_CLASS;
    InitCommonControlsEx(&ic);
    hUpDown = CreateWindowW(UPDOWN_CLASSW, NULL, WS_CHILD | WS_VISIBLE
        | UDS_SETBUDDYINT | UDS_ALIGNRIGHT,
        300, 400, 0, 0, hWnd, (HMENU)9, NULL, NULL);

    hEdit = CreateWindowExW(WS_EX_CLIENTEDGE, WC_STATICW, NULL, WS_CHILD
        | WS_VISIBLE | ES_RIGHT, 320, 400, 70, 25, hWnd,
        (HMENU)10, NULL, NULL);

    // hStatic = CreateWindowW(WC_STATICW, L"1", WS_CHILD | WS_VISIBLE
       // | SS_LEFT, 400, 402, 30, 20, hWnd, (HMENU)11, NULL, NULL);

    SendMessageW(hUpDown, UDM_SETBUDDY, (WPARAM)hEdit, 0);
    SendMessageW(hUpDown, UDM_SETRANGE, 0, MAKELPARAM(UD_MAX_POS, UD_MIN_POS));
    SendMessageW(hUpDown, UDM_SETPOS32, 0, 0);
}

void updateLabel(void) 
{
    LRESULT pos = SendMessageW(hTrack, TBM_GETPOS, 0, 0);
    wchar_t buf[4];
    wsprintfW(buf, L"%ld", pos);

    int value = (int)pos;
    algManager.setFilter(value);

    SetWindowTextW(hlbl, buf);
}