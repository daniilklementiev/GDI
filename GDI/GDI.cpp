// GDI.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GDI.h"
#include <CommCtrl.h>
#include "windowsx.h"
#include <wingdi.h>
#define MAX_LOADSTRING             100
#define CMD_BUTTON_ELLIPS          1001
#define CMD_BUTTON_RECTANGLE       1002
#define CMD_BUTTON_ROUNDRECTANGLE  1003
#define CMD_BUTTON_POLYGON         1004

// Global Variables:
HINSTANCE   hInst;                                // current instance
WCHAR       szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR       szWindowClass[MAX_LOADSTRING];            // the main window class name
bool        isLeftHold, isRightHold;
HDC         dc;
HPEN        pen;
HWND        static_pen_size;
WCHAR       str[MAX_LOADSTRING];
int         pen_size = 1;
HWND ellips;
HWND rectangle;
HWND roundrect;
HWND polygon;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GDI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDI));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(250,200,100));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GDI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE: {
        ellips = CreateWindowW(L"Button", L"Ellips", WS_CHILD | WS_VISIBLE, 10, 40, 75, 23, hWnd, (HMENU)CMD_BUTTON_ELLIPS, hInst, 0);
        rectangle = CreateWindowW(L"Button", L"Rectangle", WS_CHILD | WS_VISIBLE, 10, 70, 75, 23, hWnd, (HMENU)CMD_BUTTON_RECTANGLE, hInst, 0);
        roundrect = CreateWindowW(L"Button", L"RoundRect", WS_CHILD | WS_VISIBLE, 10, 100, 75, 23, hWnd, (HMENU)CMD_BUTTON_ROUNDRECTANGLE, hInst, 0);
        polygon = CreateWindowW(L"Button", L"Polygon", WS_CHILD | WS_VISIBLE, 10, 130, 75, 23, hWnd, (HMENU)CMD_BUTTON_POLYGON, hInst, 0);
        isLeftHold = isRightHold = false;
        dc = GetDC(hWnd);
        static_pen_size = CreateWindowW(L"Static", L"1", WS_CHILD | WS_VISIBLE, 10, 10, 50, 20, hWnd, 0, hInst, 0);
        pen = CreatePen(PS_SOLID, pen_size, RGB(250, 0, 0));
        break;
    }
    case WM_LBUTTONDOWN: {
        isLeftHold = true;
        MoveToEx(dc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), NULL);
        SetCapture(hWnd);
        break;
        
    }
    
    case WM_MOUSEWHEEL: {
        if ((int)wParam < 0 && pen_size > 1) {
            pen_size--;
            _itow_s(pen_size, str, 10);
            SendMessageW(static_pen_size, WM_SETTEXT, 100, (LPARAM)str);
        }
            
        else if ((int)wParam >= 0 && pen_size >= 1) {
            pen_size++; 
            _itow_s(pen_size, str, 10);
            SendMessageW(static_pen_size, WM_SETTEXT, 100, (LPARAM)str);
        }

        
        if (pen_size >= 1)
        {
            pen = CreatePen(PS_SOLID, pen_size, RGB(250, 0, 0));
        }
        
        break;
    }
    case WM_LBUTTONUP: {
        isLeftHold = false;
        ReleaseCapture();
        break;

    }
    case WM_RBUTTONDOWN: {
        isRightHold = true;
        MoveToEx(dc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), NULL);
        SetCapture(hWnd);
        break;

    }
    case WM_RBUTTONUP: {
        isRightHold = false;
        ReleaseCapture();
        break;

    }
    case WM_MOUSEMOVE: {
        if (isLeftHold){
            HPEN savedPen = (HPEN)SelectObject(dc, pen);
            LineTo(dc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            SelectObject(dc, pen);
            
        }
        else if (isRightHold) {
            HPEN savedPen = (HPEN)SelectObject(dc, pen);
            LineTo(dc, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            SelectObject(dc, pen);
        }
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case CMD_BUTTON_ELLIPS: {
                SendMessageW(ellips, WM_KILLFOCUS, 0, 0);
                HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
                HPEN pen = CreatePen(PS_SOLID, pen_size, RGB(255,0,0));
                SelectObject(dc, brush);
                SelectObject(dc, pen);
                Ellipse(dc, 100, 100, 200, 200);
                DeleteObject(brush);
                DeleteObject(pen);
                break;
            }
            case CMD_BUTTON_RECTANGLE: {
                SendMessageW(rectangle, WM_KILLFOCUS, 0, 0);
                HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
                HPEN pen = CreatePen(PS_SOLID, pen_size, RGB(255, 0, 0));
                SelectObject(dc, brush);
                SelectObject(dc, pen);
                Rectangle (dc, 300, 300, 200, 200);
                DeleteObject(brush);
                DeleteObject(pen);
                break;
            }
            case CMD_BUTTON_ROUNDRECTANGLE: {
                SendMessageW(roundrect, WM_KILLFOCUS, 0, 0);
                HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
                HPEN pen = CreatePen(PS_SOLID, pen_size, RGB(255, 0, 0));
                SelectObject(dc, brush);
                SelectObject(dc, pen);
                RoundRect(dc, 300, 300, 400, 400, 20, 20);
                DeleteObject(brush);
                DeleteObject(pen);
                break;
            }
            case CMD_BUTTON_POLYGON: {
                SendMessageW(polygon, WM_KILLFOCUS, 0, 0);
                POINT pt[3];
                pt[0].x = 800;
                pt[0].y = 800;
                pt[1].x = 850;
                pt[1].x = 850;
                pt[2].x = 850;
                pt[2].y = 800;
                Polygon(dc, pt, 10);
               
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        DeleteObject(pen);
        ReleaseDC(hWnd, dc);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
